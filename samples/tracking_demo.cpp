#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "tracking.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
    "{ v video        |        | video to process         }"
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

  // Do something cool.
  VideoCapture cap;
	  if (parser.has("video"))
	   cap = VideoCapture(parser.get<string>("video"));
  else
	   cap = VideoCapture(0);
  
	  if (!cap.isOpened())
	   {
	  cout << "Failed to open video capture." << endl;
	  return 1;
	 }
  
	  for (;;)
	  {
		  Mat frame;
		  cap >> frame;
		  MedianFlowTracker track;
		  track.Init(frame,Rect(25,25,50,50));
			track.Track(frame);
			rectangle(frame, track.Track(frame), CV_RGB(255, 0, 255));
			if (frame.empty()) break;
			imshow("x", frame);
			if (cv::waitKey(30) >= 0) break;
	  }
  return 0;
}
