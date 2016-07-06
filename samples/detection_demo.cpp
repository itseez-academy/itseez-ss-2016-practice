#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
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
	"{ m model        | <none> | detector model                                         }"
	"{ h ? help usage |        | print help message                       }";

void processImage(string & windowName, Mat & frame, CascadeDetector & detector) {
	vector<Rect> objects;
	vector<double> scores;
	detector.Detect(frame, objects, scores);
	Mat frame_copy = frame.clone();
	for each (Rect object in objects)
	{
		rectangle(frame_copy, object, Scalar(255, 0, 0));
	}
	imshow(windowName, frame_copy);
	waitKey(1);
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

  if (!parser.has("model")) {
	  cerr << "Specify detector model" << endl;
	  return -1;
  }

  string mfilepath = parser.get<string>("model");

  CascadeDetector detector;
  detector.CreateDetector("cascade");
  if (!detector.Init(mfilepath)) {
	  cerr << "Can't load cascade!" << endl;
	  return -1;
  }

  string windowName("Detected objects");

  if (parser.has("video")) {
	  string vfilepath = parser.get<string>("video");
	  VideoCapture cap(vfilepath);
	  Mat frame;
	  if(!cap.isOpened()) {
		  cerr << "Can't open video capture!" << parser.get<string>("video") << endl;
		  return -1;
	  }
	  cap >> frame;
	  while (frame.cols * frame.rows)
	  {
		  processImage(windowName, frame, detector);
		  cap >> frame;
	  }
  }

  return 0;
}
