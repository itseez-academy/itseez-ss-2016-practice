#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "detection.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
  "{ i image        | <none> | image to process                         }"
  "{ m model        | <none> |                                          }"
  "{ v video        | <none> | video to process                         }";

int main(int argc, const char** argv) {
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  CascadeDetector d;
  if (parser.has("i") && parser.has("m")) {
	  Mat img = imread(parser.get<string>("i"));
	  if (!d.Init(parser.get<string>("m"))) {
		  cout << "Not correct model way";
		  return 0;
	  }
	  std::vector<cv::Rect> objects;
	  std::vector<double> scores;
	  d.Detect(img, objects, scores);
	  int size = objects.size();

	  namedWindow("Image", WINDOW_NORMAL);
	  resizeWindow("Image", 640, 480);
	  imshow("Image", img);  

	  for (int i = 0; i < size; i++)
		  rectangle(img, objects[i], Scalar(0,0,0,0));

	  namedWindow("ImageNew", WINDOW_NORMAL);
	  resizeWindow("ImageNew", 640, 480);
	  imshow("ImageNew", img);

	  waitKey();
  }
  Mat img;
  if (parser.has("v") && parser.has("m")) {
	  VideoCapture cap(parser.get<string>("v"));
	  for (;;)
	  {
		  cap >> img;
		  if (img.empty())
			  break;

		  if (!d.Init(parser.get<string>("m"))) {
			  cout << "Not correct model way";
			  return 0;
		  }
		  std::vector<cv::Rect> objects;
		  std::vector<double> scores;
		  d.Detect(img, objects, scores);
		  int size = objects.size();
		  for (int i = 0; i < size; i++)
			  rectangle(img, objects[i], Scalar(0, 0, 0, 0));
		  namedWindow("ImageNew", WINDOW_NORMAL);
		  imshow("ImageNew", img);
		  if (waitKey(30) > 0) break;
	  }
  }

  return 0;
}
