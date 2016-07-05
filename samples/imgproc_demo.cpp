#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "image_processing.hpp"

using namespace std;
using namespace cv;

const char* kAbout = "Application for practice #2.";

const char* kOptions =
"{ @image         |        | image to process         }"
"{ t              |  128   | threshold                }"
"{ h ? help usage |        | print help message       }";


int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  Mat src = imread(parser.get<string>(0));
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

  Rect roi(100, 200, 100, 100);
  ImageProcessorImpl imgproc;

  // Show destination image.
  const string kDstWindowName1 = "Destination image 1";
  namedWindow(kDstWindowName1, WINDOW_NORMAL);
  resizeWindow(kDstWindowName1, 640, 480);
  imshow(kDstWindowName1, imgproc.CvtColor(src, roi));

  int kSize = 9 * 9;

  // Show destination image.
  const string kDstWindowName2 = "Destination image 2";
  namedWindow(kDstWindowName2, WINDOW_NORMAL);
  resizeWindow(kDstWindowName2, 640, 480);
  imshow(kDstWindowName2, imgproc.Filter(src, roi, kSize));
  
  int filterSize = 7;
  int lowThreshold = 1;
  int ratio = 50;
  int kernelSize = 3;
  // Show destination image.
  const string kDstWindowName3 = "Destination image 3";
  namedWindow(kDstWindowName3, WINDOW_NORMAL);
  resizeWindow(kDstWindowName3, 640, 480);
  imshow(kDstWindowName3, imgproc.DetectEdges(src, roi, filterSize, lowThreshold, ratio, kernelSize));

  waitKey();
  return 0;
}
