#include <iostream>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

#include "detection.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "detection app";

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

  // init
  std::string vidName, imgName, detectName;
  Mat frame;
  CascadeDetector obj;
  std::vector<Rect> logos;
  std::vector<double> scores;
  VideoCapture capture;
  //
 
  if (parser.has("m"))
  {
	  detectName = parser.get<string>("m");
	  cout << detectName << endl;
	  obj.Init(detectName);
  }
  
  if (parser.has("i"))
  {
	  imgName = parser.get<std::string>("i");
	  frame = imread(imgName);
	  imshow("Detection", frame);
	  obj.Detect(frame, logos, scores);
	  if (scores.size() > 0)
		  for (int i = 0; i < logos.size(); i++)
		  {
			  rectangle(frame, logos[i], Scalar(250, 150, 10));
			  imshow("Detection", frame);
		  }
	  else
		  std::cout << "objects haven`t been detected";
  }
  else
  {
	  if (parser.has("v"))
	  {		
		  vidName = parser.get<std::string>("v");
		  
		  bool flag = capture.open(vidName);
		  
		  if (flag)
		  {	
			  flag = capture.read(frame);
			  while (true)
			  {
				 if (frame.empty())
					 {
						 printf(" --(!) No captured frame -- Break!");
						 break;
					 }
				 //Apply the classifier to the frame
			     obj.Detect(frame, logos, scores);
				 if (scores.size()> 0)
		        	 for(int i = 0; i < logos.size(); i++)
	   					rectangle(frame, logos[i], Scalar(250, 150, 10));
                 imshow("Detection", frame);
			     capture >> frame;
				 int c = cv::waitKey(100);
			     if ((char)c == 27) break;  // escape	
			  }
			  capture.release();
		  }
		  else
			  cout << "--(!)Error opening video capture" << endl ;
		  
	  }
	  else
		  if (parser.has("c"))
		  {
			  capture.open(0);
			  if (!capture.isOpened())
				   return 0;
			  capture.read(frame);
			  for (;;)
			  {	
				  obj.Detect(frame, logos, scores);
				  if (scores.size() > 0)
					  for (int i = 0; i < logos.size(); i++)
						  rectangle(frame, logos[i], Scalar(250, 150, 10));
				  capture >> frame;
				  if (frame.empty()) break; // end of video stream
				  imshow("Web", frame);
				  if (waitKey(10) == 27) break; // stop capturing by pressing ESC 
			  }
			  capture.release();
		  }
  }
  cv::waitKey(0);

  return 0;
}
