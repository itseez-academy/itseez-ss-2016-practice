#include <iostream>
#include <string>


#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "image_processing.hpp"

using namespace std;
using namespace cv;

const char* kAbout =   "Application for practice #2.";

const char* kOptions =
	"{ @image        | <none> | image to process           }"
    "{ gray          |        | convert ROI to gray scale  }"
	"{ median        |        | apply median filter for ROI}"
	"{ edges         |        | detect edges in ROI        }"
	"{ pix           |        | pixelize ROI               }"
	"{ h ? help usage|        | print help message         }";


int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  // Read image.
  Mat src = imread(parser.get<string>(0), CV_LOAD_IMAGE_GRAYSCALE);
  if (src.empty()) {
	  cout << "Failed to open image file '" + parser.get<string>(0) + "'."
		  << endl;
	  return 0;
  }

  // Show source image.
  const string kSrcWindowName = "Source image";
  const int kWaitKeyDelay = 1;
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  resizeWindow(kSrcWindowName, 640, 480);
  imshow(kSrcWindowName, src);
  waitKey(kWaitKeyDelay);

  ImageProcessorImpl processor;
  Rect roi(20, 20, 220, 220);
  processor.CvtColor(src, roi);
 // processor.Filter(src, roi,3);

  // Show destination image.
  const string kDstWindowName = "Destination image";
  namedWindow(kDstWindowName, WINDOW_NORMAL);
  resizeWindow(kDstWindowName, 640, 480);
  imshow(kDstWindowName, src);
  waitKey();

  // Do something cool.
  cout << "This is empty template sample." << endl;

  return 0;
}
