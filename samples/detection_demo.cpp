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

  /*Mat src = imread(parser.get<string>("i"));
  if (src.empty()) {
	  cout << "Failed to open image file '" + parser.get<string>("i") + "'." << endl;
	  return 0;
  }*/
	  
  VideoCapture capture;
  Mat frame;
  //capture.open(0);	//������
  capture.open("logo.mp4");
  if (!capture.isOpened()) 
  { 
	  printf("--(!)Error opening video capture\n"); return -1; 
  }
  while (capture.read(frame))
  {
	  if (frame.empty())
	  {
		  printf(" --(!) No captured frame -- Break!");
		  break;
	  }
	  //-- 3. Apply the classifier to the frame
	  CascadeDetector detector;
	  vector<Rect> obj;
	  detector.Init(parser.get<string>("m"));
	  detector.Detect(frame, obj);

	  for (const auto& r : obj)
		  rectangle(frame, r, CV_RGB(255, 0, 255));
	  imshow("x", frame);
	  int c = waitKey(1);
	  if ((char)c == 27) { break; } // escape
  }
  /*const string kSrcWindowName = "Source image";
  const int kWaitKeyDelay = 1;
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  resizeWindow(kSrcWindowName, 640, 480);
  imshow(kSrcWindowName, src);*/

  // Do something cool.
  /*CascadeDetector detector;
  vector<Rect> obj;
  detector.Init(parser.get<string>("m"));
  detector.Detect(src, obj);

  for (const auto& r:obj)
	  rectangle(src, r , CV_RGB(255, 0, 255));
  imshow("x", src);*/
  waitKey();
  return 0;
}
