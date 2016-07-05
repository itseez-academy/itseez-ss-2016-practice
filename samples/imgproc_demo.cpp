#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "image_processing.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
	"{ @image         | <none> | image to process            }"
	"{ v video        | <none> | video to process            }"
	"{ gray           |        | convert ROI to gray scale   }"
	"{ median         |        | apply median filter for ROI }"
	"{ edges          |        | detect edges in ROI         }"
	"{ pix            |        | pixelize ROI                }"
    "{ h ? help usage |        | print help message          }";

struct MousePosition {
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
} mouse;

static void OnMouse(int event, int x, int y, int, void*) {
	switch (event) {
	case EVENT_LBUTTONDOWN :
		mouse.is_selection_started = true;
		mouse.is_selection_finished = false;
		mouse.point_first = Point(x, y);
		break;
	case EVENT_LBUTTONUP :
		mouse.is_selection_started = true;
		mouse.is_selection_finished = true;
		mouse.point_second = Point(x, y);
		break;
	case EVENT_MOUSEMOVE :
		if (mouse.is_selection_started && !mouse.is_selection_finished)
			mouse.point_second = Point(x, y);
	}
}

const int medianSize = 5;
const int edgesFilterSize = 5;
const int edgesLowThreshold = 100;
const int edgesRatio = 2;
const int edgesKernelSize = 5;
const int pixDivs = 10;

int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

	Mat src, dst;
  VideoCapture cap(argv[1]);
  cap >> src;
  if (src.empty()) {
	  cout << "Failed to open image file '" + parser.get<string>(0) + "'."
	  << endl;
	  return 0;
  }

  const string kSrcWindowName = "Source image";
  const int kWaitKeyDelay = 1;
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  setMouseCallback(kSrcWindowName, OnMouse, 0);
  resizeWindow(kSrcWindowName, 640, 480);
  imshow(kSrcWindowName, src);
  waitKey(kWaitKeyDelay);

  mouse.is_selection_started = false;
  mouse.is_selection_finished = false;

  while (!mouse.is_selection_finished) {
	  waitKey(30);
  }

  Rect roi(mouse.point_first.x, mouse.point_first.y, 
	  mouse.point_second.x - mouse.point_first.x, 
	  mouse.point_second.y - mouse.point_first.y);
  rectangle(src, roi, Scalar(254));

  ImageProcessorImpl proc;
  bool firstFrame = true;
  const string kDstWindowName = "Destination image";
  namedWindow(kDstWindowName, WINDOW_NORMAL);
  resizeWindow(kDstWindowName, 640, 480);
  for (;;) {
	  if (src.empty() && !firstFrame)
		  break;

	  if (parser.get<bool>("gray"))
	  {
		  dst = proc.CvtColor(src, roi);
	  }

	  if (parser.get<bool>("median"))
	  {
		  dst = proc.Filter(src, roi, medianSize);
	  }

	  if (parser.get<bool>("edges"))
	  {
		  dst = proc.DetectEdges(src, roi, edgesFilterSize, edgesLowThreshold,
			  edgesRatio, edgesKernelSize);
	  }

	  if (parser.get<bool>("pix"))
	  {
		  dst = proc.Pixelize(src, roi, pixDivs);
	  }

	  firstFrame = false;
	  cap >> src;
	  imshow(kDstWindowName, dst);
	  if(waitKey(30) >= 0) break;
  }
  waitKey();
  return 0;
}
