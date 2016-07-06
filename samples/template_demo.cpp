#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "image_processing.hpp"

#include <ctype.h>

using namespace std;
using namespace cv;

Mat image;
Mat source;

bool backprojMode = false;
bool selectObject = false;
int trackObject = 0;
bool showHist = true;
Point origin;
Rect selection;
bool gray = false;
bool filter = false;
bool edge = false;
bool pixel = false;

void printNewImage();

static void onMouse(int event, int x, int y, int, void*)
{
	if (selectObject)
	{
		source.copyTo(image);
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = std::abs(x - origin.x);
		selection.height = std::abs(y - origin.y);
		selection &= Rect(0, 0, image.cols, image.rows);
		rectangle(image, selection, Scalar(115, 115, 115));
		imshow("Source image", image);
	}

	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		origin = Point(x, y);
		selection = Rect(x, y, 0, 0);
		selectObject = true;
		break;
	case EVENT_LBUTTONUP:
		selectObject = false;
		if (selection.width > 0 && selection.height > 0)
			trackObject = -1;
		//cout << selection.height << endl;
		source.copyTo(image);
		printNewImage();
		break;
	}
}

void printNewImage()
{
	/*Mat dest;
	//dest.b
	image.copyTo(dest);
	const string kSrcWindowName = "Destination_image";
	const int kWaitKeyDelay = 0;
	namedWindow(kSrcWindowName, WINDOW_NORMAL);
	resizeWindow(kSrcWindowName, 640, 480);
	//dest = image(selection);
	ImageProcessor1 pr;
	if (gray)
		dest = pr.CvtColor(dest, selection);
	else
	if (filter)
		dest = pr.Filter(dest, selection,7);
	if (edge)
		dest = pr.DetectEdges(dest, selection, 5, 3, 7, 3);
	else
		if (pixel)
			dest = pr.Pixelize(dest, selection, 10);
	//IplImage* img = cvCloneImage(&(IplImage)dest);
	//cvSetImageROI(img, selection);
	//cvAddS(img, cvScalar(17), img);
	//cvShowImage("ROI", img);
	resizeWindow(kSrcWindowName, 640, 480);

	imshow(kSrcWindowName, dest);**/
}

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";


const char* kOptions =
"{ @image         | <none> | image to process            }"
"{ gray ? gray    |        | convert ROI to gray scale   }"
"{ median         |        | apply median filter for ROI }"
"{ edges          |        | detect edges in ROI         }"
"{ pix            |        | pixelize ROI                }"
"{ v video        |        | video to process         }"
"{ h ? help usage |        | print help message          }";



int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  source = imread(parser.get<string>(0));// CV_LOAD_IMAGE_GRAYSCALE);
  if (parser.get<string>(1) == "gray")
	  gray = true;
  if (parser.get<string>(1) == "median")
	  filter = true;
  if (parser.get<string>(1) == "edges")
	  edge = true;
  if (parser.get<string>(1) == "pix")
	  pixel = true;
  source.copyTo(image);
  if (image.empty()) {
	  cout << "Failed to open image file '" + parser.get<string>(0) + "'."
		  << endl;
	  return 0;
  }
  
  // Show source image.
  const string kSrcWindowName = "Source image";
  const int kWaitKeyDelay = 0;
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  resizeWindow(kSrcWindowName, 640, 480);
  imshow(kSrcWindowName, image);
  setMouseCallback(kSrcWindowName, onMouse, 0);
  waitKey(kWaitKeyDelay);

  // Do something cool.
  cout << "This is empty template sample." << endl;

  return 0;
}
