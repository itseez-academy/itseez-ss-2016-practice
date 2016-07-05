#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

#include "image_processing.hpp"


const char* kAbout =
    "This is image processing test application";

const char* kOptions =
	"{ @image         |        | image to process         }"
	"{ h ? help usage |        | print help message       }";

int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.has("help")) {
	  parser.printMessage();
	  return 0;
  }

  // Read image.
  Mat src = imread(parser.get<string>(0));
  if (src.empty()) {
	  cout << "Failed to open image file '" + parser.get<string>(0) + "'."
		  << endl;
	  return 0;
  }

  ImageProcessorImpl proc;
  Mat res = proc.CvtColor(src, Rect(0, 0, src.cols-30, src.rows-30));

  // Show source image.
  const string kSrcWindowName = "Source image";
  imshow(kSrcWindowName, src);
  const string kResWindowName = "Processed image";
  imshow(kResWindowName, res);
  const int kWaitKeyDelay = 0;
  waitKey(kWaitKeyDelay);

  return 0;
}
