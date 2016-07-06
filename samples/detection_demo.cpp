#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "detection.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <vector>

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
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  CascadeDetector detector;

  Mat src;
  src = imread(parser.get<string>("image"), 1);
  if (src.empty()) {
	  cout << "Failed to open image file '" + parser.get<string>(0) + "'."
		   << endl;
	  return 0;
  }

  vector<Rect> objects;
  vector<double> scores;
  detector.Init(parser.get<string>("model"));
  detector.Detect(src, objects, scores);

  for (int i = 0; i < objects.size(); i++)
	  rectangle(src, objects[i], Scalar(0));
  
  const string kSrcWindowName = "Source image";
  const int kWaitKeyDelay = 1;
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  resizeWindow(kSrcWindowName, 640, 480);
  imshow(kSrcWindowName, src);
  waitKey();

  return 0;
}
