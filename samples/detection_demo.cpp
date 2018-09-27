#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
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
"{ m model        | <none> | path to detector file                    }"
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

  // Do something cool.
  String vidName = "C:\\video\\logo.mp4";
  VideoCapture capture;
 bool f1 =  capture.open(vidName);
 if (!f1) cout << "no video" << endl;
  Mat frame;
  CascadeDetector obj;
  std::vector<Rect> logos;
  std::vector<double> scores;
//load
  String fileName = "C:\MyProjects\itseez-ss-2016-practice\test\test_data\detection\cascades\unn_old_logo_cascade.xml";
  obj.Init(fileName);
   //read 
//  capture.open(0);
//  if (!capture.isOpened()) { cout<<"--(!)Error opening video capture" << endl; return -1; }
  bool f = capture.read(frame);
  if (!f) cout <<  "no video" << endl;
  while (true)
  {
	  if (frame.empty())
	  {
		  printf(" --(!) No captured frame -- Break!");
		  break;
	  }

	  //-- 3. Apply the classifier to the frame
	  obj.Detect(frame,logos,scores);
	 // rectangle(frame, obj, Scalar(250, 150, 10));
	  imshow("logos", frame);
	  capture >> frame;
	  int c = cv::waitKey(100);
	  if ((char)c == 27) { break; } // escape
  }
  capture.release();
  //cv::waitKey(0);
  return 0;
}
