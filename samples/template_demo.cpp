#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "detection.hpp"


using namespace std;
using namespace cv;


/*
const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
"{@image        |      | image to process        }"
"{ @ model      |  128   | threshold                }"
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

   // Read image.
 // if (parser.get<bool>("i")) {
	  Mat src = imread(parser.get<string>(0), CV_LOAD_IMAGE_COLOR);

	  if (src.empty()) {
		  cout << "Failed to open image file '" + parser.get<string>(0) + "'."
			  << endl;
		  return 0;
	  }
 // }

  
 CascadeDetector detector;
 string model = parser.get<string>(1);

 detector.Init(model);

 std::vector<Rect> objects;
 detector.Detect(src, objects, detector.scores);

 // for (int i = 0; i < sizeof(detector.scores); i++) {
	 // Draw a rectangle 
	// rectangle(src, mouse.point_first, mouse.point_second, Scalar(0, 55, 255));

 //}

 return 0;
}
*/