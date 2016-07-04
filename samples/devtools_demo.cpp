#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "workaround.hpp"

using namespace std;
using namespace cv;

const char* kAbout = "Application for practice #1.";

const char* kOptions =
    "{ @image         |        | image to process         }"
    "{ t              |  128   | threshold                }"
	"{ s              |  2    | surroundings             }"
    "{ h ? help usage |        | print help message       }";

int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.has("help")) {
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

  // Threshold data.
  MatrixProcessor processor;
  const int threshold = parser.get<int>("t");
  try {
    processor.Threshold(src.data, src.cols, src.rows, threshold);
  } catch (const std::exception& ex) {
    cout << ex.what() << endl;
    return 0;
  }

  // Show destination image.
  const string kDstWindowName = "Destination image";
  namedWindow(kDstWindowName, WINDOW_NORMAL);
  resizeWindow(kDstWindowName, 640, 480);
  imshow(kDstWindowName, src);

  // Averaging data.
  const int surroundings = parser.get<int>("s");
  Mat src2 = imread(parser.get<string>(0), CV_LOAD_IMAGE_GRAYSCALE);
  try {
	  processor.Averaging(src2.data, src2.cols, src2.rows, surroundings);
  }
  catch (const std::exception& ex) {
	  cout << ex.what() << endl;
	  return 0;
  }

  // Show Averaging image.
  const string kDstWindowName2 = "Averaged image";
  namedWindow(kDstWindowName2, WINDOW_NORMAL);
  resizeWindow(kDstWindowName2, 640, 480);
  imshow(kDstWindowName2, src2);

  // Median data.
  Mat src3 = imread(parser.get<string>(0), CV_LOAD_IMAGE_GRAYSCALE);
  try {
	  processor.Median(src3.data, src3.cols, src3.rows, surroundings);
  }
  catch (const std::exception& ex) {
	  cout << ex.what() << endl;
	  return 0;
  }

  // Show Median image.
  const string kDstWindowName3 = "Median image";
  namedWindow(kDstWindowName3, WINDOW_NORMAL);
  resizeWindow(kDstWindowName3, 640, 480);
  imshow(kDstWindowName3, src3);
  waitKey();

  return 0;
}
