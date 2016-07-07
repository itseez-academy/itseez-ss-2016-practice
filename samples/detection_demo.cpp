#include <iostream>
#include <string>
#include "detection.hpp"
#include "opencv2/core.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
"{ i image        | <none> | image to process                         }"
"{ v video        | <none> | video to process                         }"
"{ c camera       | <none> | camera to get video from                 }"
"{ m model        | <none> |                                          }"
"{ h ? help usage |        | print help message                       }";
//string path;

int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  CascadeDetector detector;
  detector.Init("C:/OpenCV31/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml");
 //detector.Init("/Users/iss2016/Documents/summer_school_emelin/itseez-ss-2016-practice/test/test_data/detection/cascades/opencv_logo_cascade.xml");
 // Mat src = imread("/Users/iss2016/Documents/summer_school_emelin/itseez-ss-2016-practice/test/test_data/imgproc/logo.png");
  VideoCapture cap(0);
  vector<Rect> dts;
  vector<double> score;
  

  const string kSrcWindowName = "Destination image";
  const int kWaitKeyDelay = 1;
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  resizeWindow(kSrcWindowName, 640, 480);

  for (;;)
  {
	  Mat frames;
	  cap >> frames;
	  detector.Detect(frames, dts, score);
	  for (int i = 0; i < dts.size(); i++)
	  {
		  cv::rectangle(frames, dts[i], CV_RGB(0, 255, 0), 10);
	  }
	  imshow(kSrcWindowName, frames);
	  if (waitKey(kWaitKeyDelay) >= 0) break;
  }
  waitKey();
  return 0;
}
