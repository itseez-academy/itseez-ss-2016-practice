#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"



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

  // Do something cool.

 
  // Read image.
  Mat src = imread("C://Users/iss2016/Andreeva/itseez - ss - 2016 - practice/test/test_data/detection/positives/intel_logo.png", CV_LOAD_IMAGE_COLOR);

  cout << "Read image" << endl;

  if (src.empty()) {
	  cout << "Failed to open image file '" + parser.get<string>(0) + "'."
		  << endl;
	  return 0;
  }


  cout << "This is empty template sample." << endl;

  return 0;
}
