#include <iostream>
#include <string>

#include "detection.hpp"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

const char* kAbout = "Practice 3";

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

  if (parser.has("image"))
  {
	  string imagePath = parser.get<string>("image");

	  Mat image = imread(imagePath);
	  if (image.empty()) {
		  cout << "Failed to open image file '" + imagePath + "'."
			  << endl;
		  return 1;
	  }

	  const string windowName = "Your image";
	  namedWindow(windowName, WINDOW_NORMAL);
	  resizeWindow(windowName, image.cols, image.rows);
	  imshow(windowName, image);
	  waitKey();
  }

  //TODO: allow video
  //TODO: allow camera



  return 0;
}
