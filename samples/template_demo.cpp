#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "image_processing.hpp"


using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
	"{ @image         |        | image to process         }"
    "{ v video        |        | video to process         }"
    "{ h ? help usage |        | print help message       }";

struct MouseCallbackState {
	bool is_selection_started;
	bool is_selection_finished = false;
	Point point_first;
	Point point_second;
};

MouseCallbackState mouse;
Rect selection;

static void onMouse(int event, int x, int y, int, void*)
{
	switch (event)
		{
		case EVENT_LBUTTONDOWN:
			mouse.is_selection_started = true;
			mouse.is_selection_finished = false;
			mouse.point_first = Point(x, y);
			break;
		case EVENT_LBUTTONUP:
			mouse.is_selection_started = false;
			mouse.is_selection_finished = true;
			mouse.point_second = Point(x, y);

			break;
		case EVENT_MOUSEMOVE:
			if (mouse.is_selection_finished = false)
			{
				mouse.point_second = Point(x, y);
			}
			break;
		}
	
}


int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }


  Mat src = imread(parser.get<string>(0), CV_LOAD_IMAGE_COLOR);
  if (src.empty()) {
	  cout << "Failed to open image file '" + parser.get<string>(0) + "'."
		  << endl;
	  return 0;
  }
  
  Mat copy_src;
  src.copyTo(copy_src);

  // Show source image.
  const string kSrcWindowName = "Source image";
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  resizeWindow(kSrcWindowName, 640, 480);
  imshow(kSrcWindowName, src);
  

  
  setMouseCallback("Source image", onMouse, 0);
  while (cv::waitKey(1) != 27);

  cout << mouse.point_first << endl;
  cout << mouse.point_second << endl;
  
  ////


  Point pTopLeft = mouse.point_first;
  Point pBottomRight = mouse.point_second;
  cv::Rect selection(pTopLeft.x, pTopLeft.y, pBottomRight.x - pTopLeft.x, pBottomRight.y - pTopLeft.y);


  ///


  // Draw a rectangle 
  rectangle(copy_src, mouse.point_first, mouse.point_second, Scalar(0, 55, 255));

  
 // imshow("Copy Image", copy_src);




  // Do something cool.

  Mat im_grayscale;
  
  ImageProcessorImpl imageprocessor;
  im_grayscale = imageprocessor.CvtColor(copy_src, selection);



  imshow("Gray scale", im_grayscale);

  while (cv::waitKey(1) != 27);



  return 0;
}
