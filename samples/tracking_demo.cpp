#include <iostream>
#include <string>

#include "opencv2/core.hpp"
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
  MedianFlowTracker tracker;
  //VideoCapture cap("Users/iss2016/Documents/itseez-ss-2016-practice/test/test_data/video/logo.mp4");
  Mat frame_prev,frame_next;
  VideoCapture cap(0);
  Rect roi(10, 20, 50, 100);

	cap >> frame_prev;
	tracker.Init(frame_prev, roi);
  
  for (;;) {
	  cap >> frame_next;

	  rectangle(frame_next, tracker.Track(frame_next),CV_RGB(0,240,0));
	
	  tracker.Init(frame_next, roi);


	  imshow("Tracking", frame_next);
	  if(waitKey(30)>=0)break;
  }



  // Do something cool.
  cout << "This is empty template sample." << endl;

  return 0;
}
