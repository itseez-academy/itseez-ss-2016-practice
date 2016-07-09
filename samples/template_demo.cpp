#include <iostream>
#include <string>

#include "opencv2/core.hpp"

#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
	const int height, const int threshold) {
	// TODO: Add thresholding logic here.
}

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
    "{ v video        |        | video to process         }"
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

 // MatrixProcessor::Threshold()
	//  Mat src = imread(parser.get<string>(0), CV_LOAD_IMAGE_GRAYSCALE);

  return 0;
}
