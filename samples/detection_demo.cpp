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


int detectOnImage(Mat& frame, CascadeDetector& detector);

int main(int argc, const char** argv) {
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  CascadeDetector detector;
  detector.Init(parser.get<string>("model"));

  if (parser.has("image")) {
	  Mat src;
	  src = imread(parser.get<string>("image"), 1);
	  if (src.empty()) {
		  cout << "Failed to open image file '" + parser.get<string>(0) + "'."
			  << endl;
		  return 0;
	  }
	  detectOnImage(src, detector);
	  waitKey();
  }
  if (parser.has("video")) {
	  VideoCapture cap(parser.get<string>("video"));
	  if (!cap.isOpened())
		  cout << "Error!" << endl;
	  for (;;) {
		  Mat src;
		  cap >> src;
		  if (src.empty())
			  break;
		  detectOnImage(src, detector);
		  if (waitKey(30) >= 0)
			  break;
	  }
  }

  return 0;
}

int detectOnImage(Mat& frame, CascadeDetector& detector) {
	vector<Rect> objects;
	vector<double> scores;
	detector.Detect(frame, objects, scores);

	for (int i = 0; i < objects.size(); i++)
		rectangle(frame, objects[i], Scalar(0));

	const string kSrcWindowName = "Source image";
	const int kWaitKeyDelay = 1;
	namedWindow(kSrcWindowName, WINDOW_NORMAL);
	resizeWindow(kSrcWindowName, 640, 480);
	imshow(kSrcWindowName, frame);
	return 0;
}
