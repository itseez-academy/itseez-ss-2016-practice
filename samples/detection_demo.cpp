#include <iostream>
#include <string>
#include"detection.hpp"
#include "opencv2/core.hpp"

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
 
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);
  CascadeDetector cd;
  cd.Init("C:/Temp/proj1/build/bin/unn_old_logo_cascade.XML");
  Mat img;
  if (parser.get<bool>("image")) {
	  cout << argv[2] << endl;

	  cout << "IMAGE" << endl;
	  img = imread(argv[2]);
	  if (img.empty())
	  {
		  cout << "Error loading the image" << endl;
		  return -1;
	  }
	  vector<Rect> objects;
	  vector<double> scores;
	  cd.Detect(img, objects, scores);
	  cout << "size = " << objects.size() << endl;
	  for (size_t i = 0; i < objects.size(); i++)
	  {
		  cv::rectangle(img, objects[i], Scalar(0, 0, 0, 0));

	  }

	  namedWindow("Window1", 1);
	  imshow("Window1", img);

	  waitKey(0);

    return 0;
  }
  if (parser.get<bool>("video")) {
	  namedWindow("Window1", 1);

	  VideoCapture cap(argv[2]);

	  for (;;) {
		  cap >> img;
		  if (img.empty())
		  {
			  cout << "Error loading the image" << endl;
			  return -1;
		  }
		  imshow("Window1", img);

		  vector<Rect> objects;
		  vector<double> scores;
		  cd.Detect(img, objects, scores);
		  for (size_t i = 0; i < objects.size(); i++)
		  {
			  cv::rectangle(img, objects[i], Scalar(0, 0, 0, 0));
		  }
		  waitKey(30);
		  if (waitKey(30) >= 0) break;

	  }
  }

  waitKey(0);

  return 0;
}
