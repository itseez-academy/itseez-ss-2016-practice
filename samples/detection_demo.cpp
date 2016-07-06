#include <iostream>
#include <string>
#include"detection.hpp"
#include "opencv2/core.hpp"
#include"opencv2/imgproc.hpp"
#include"opencv2/highgui.hpp"
#include"opencv2/objdetect.hpp"
#include<memory>
using namespace std;
//using namespace cv;

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
  cv::CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  // Do something cool.
  std::shared_ptr<Detector> detector = CascadeDetector::CreateDetector("cascade");
  (*detector).Init("haarcascade_frontalface_default.xml");
  cv::Mat frame = cv::imread("lena.png");
  std::vector<cv::Rect> objects;
  std::vector<double> score;
/*
 (*detector).Detect(frame,objects,score);
  cout << objects.size();
  for (auto i = objects.begin(); i != objects.end(); i++)
  {
	  cv::rectangle(frame, *i, cv::Scalar(0, 0, 255));
  }

 
  
  cv::imshow("win", frame);
  cv::waitKey();
*/
 cv::VideoCapture capture(0);
 cout << capture.isOpened();
 for (;;)
 {
	 capture >> frame;
	 (*detector).Detect(frame, objects, score);
	 cout << objects.size();
	 if (frame.empty())
	 {
		 cout << "End of Sequence" << endl;
		 break;
	 }
	 for (auto i = objects.begin(); i != objects.end(); i++)
	 {
		 cv::rectangle(frame, *i, cv::Scalar(0, 0, 255));
	 }
	 imshow("Win", frame);

	 if (cv::waitKey(200) == 27)
		 break;
 }
  return 0;
}
