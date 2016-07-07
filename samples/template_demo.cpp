#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

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


  string filename = "pedestrians.mpg";

  VideoCapture capture(filename);
  Mat frame;
  Mat next_frame;

  if (!capture.isOpened())
	  throw "Error when reading steam_avi";

  
  MedianFlowTracker medianflowtraacker;

  Rect ROI(70, 70, 90, 90);
   
  const cv::Rect &roi = ROI;
  
  namedWindow("video", 1);
  for (; ; )
  {   
	  capture >> frame;

	  if (frame.empty()) {
		 continue;
	  }
	
	  //getting next frame
	  capture.read(next_frame);

	  medianflowtraacker.Init(frame, next_frame, roi);
 	  
	  imshow("video", frame);
	  waitKey(20); 
  }
  waitKey(0);



  // Do something cool.
  cout << "This is empty template sample." << endl;

  return 0;
}
