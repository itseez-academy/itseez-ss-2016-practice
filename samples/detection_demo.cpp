#include <iostream>
#include <string>

#include "opencv2/core.hpp"
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

 /* Mat src = imread(parser.get<string>("image"));// C:\Users\iss2016\Documents\itseez - ss - 2016 - practice - build\bin
  if (src.empty()) {
	  cout << "Failed to open image file '" + parser.get<string>(0) + "'."
		  << endl;
	  return 0;
  }*/
  
  // Do something cool.
  CascadeDetector cascade_detector;
 // string file_path = ;
  vector <Rect> objects;
  vector <double> score;
  VideoCapture cap(0);


 // cascade_detector.Init("/Users/iss2016/Documents/itseez-ss-2016-practice/test/test_data/detection/cascades/opencv_logo_cascade.xml");
  cascade_detector.Init("/OpenCV31/opencv/sources/data/haarcascades/haarcascade_eye.xml");
  for (;;)
  {
	  Mat frame;
	  cap >> frame;
	  cascade_detector.Detect(frame, objects, score);
	  for (int i = 0; i < objects.size(); i++)
	  {
		  rectangle(frame, objects[i], CV_RGB(255, 0, 0), 5);
	  }
	  imshow("result", frame);
	  if(waitKey(30)>=0) break;
  }

  return 0;
}
