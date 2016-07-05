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
	"{ @image         | <none> | image to process            }"
	"{ gray           |        | convert ROI to gray scale   }"
	"{ median         |        | apply median filter for ROI }"
	"{ edges          |        | detect edges in ROI         }"
	"{ pix            |        | pixelize ROI                }"
	"{ h ? help usage |        | print help message          }";

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
  Mat res = src.clone();
  if (parser.has("gray")) {
    res = proc.CvtColor(src, Rect(0, 0, src.cols - 30, src.rows - 30));
  }
  else if (parser.has("median")) {
	res = proc.Filter(src, Rect(10, 20, src.cols - 50, src.rows - 50), 11);
  }
  else if (parser.has("edges")) {
	  int filterSize = 5;
	  int lowThreshold = 50;
	  int ratio = 3;
	  int kernelSize = 5;
	  res = proc.DetectEdges(src, Rect(10, 20, src.cols - 50, src.rows - 50),
		  filterSize, lowThreshold, ratio, kernelSize);
  }
  else if (parser.has("pix")) {
	  res = proc.Pixelize(src, Rect(10, 20, src.cols - 50, src.rows - 50), 5);
  }
 
  // Show images.
  const string kSrcWindowName = "Source image";
  imshow(kSrcWindowName, src);
  const string kResWindowName = "Processed image";
  imshow(kResWindowName, res);
  const int kWaitKeyDelay = 0;
  waitKey(kWaitKeyDelay);

  return 0;
}
