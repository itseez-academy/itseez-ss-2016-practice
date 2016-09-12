#include <iostream>
#include <string>

#include "opencv2/opencv.hpp"
#include "detection.hpp"

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

  string src = parser.get<string>("m");
  Mat frame;//= imread(parser.get<string>("v"));
  vector<Rect> obj;
  VideoCapture cap;
  vector<double> sc;
  CascadeDetector DETECTOR;
  cap.open(-1);
  DETECTOR.Init(src);
  DETECTOR.Detect(frame, obj, sc);
  while (cap.read(frame)) {


	  for (const auto& r : obj)
		  rectangle(frame, r, CV_RGB(255, 0, 0));
	  imshow("x", frame);
	  waitKey();
	  int c = waitKey(10);
	  if ((char)c == 27) { break; } // escape
  }
  return 0;
}
