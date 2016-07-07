#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "workaround.hpp"

using namespace std;
using namespace cv;

const char* kAbout = "Application for practice #1.";

const char* kOptions =
    "{ @image         |        | image to process         }"
    "{ t              |  128   | threshold                }"
    "{ h ? help usage |        | print help message       }";

void addF(unsigned char* const data, unsigned char* const data1, const int width,
	const int height, const int threshold, int g) {



	for (int k = g*height; k < height*(width - g); k++)
	{
		int j = k % width;
		int i = k / width;
		data1[k] = (data[(i - g)*width + j - g] + data[(i - g)*width + j] + data[(i - g)*width + j + g]+
			data[(i)*width + j - g]		 + data[k] 		+ data[(i)*width + j + g]+
			data[(i + g)*width + j - g] + data[(i + g)*width + j ] + data[(i + g)*width + j + g]) / 9;
	}
}

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
  Mat dest;
  Mat src = imread(parser.get<string>(0), CV_LOAD_IMAGE_GRAYSCALE);
  src.copyTo(dest);	
  if (src.empty()) {
    cout << "Failed to open image file '" + parser.get<string>(0) + "'."
         << endl;
    return 0;
  }

  // Show source image.
  const string kSrcWindowName = "Source image";
  const int kWaitKeyDelay = 1;
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  resizeWindow(kSrcWindowName, 640, 480);
  imshow(kSrcWindowName, src);
  waitKey(kWaitKeyDelay);

  // Threshold data.
  MatrixProcessor processor;
  const int threshold = parser.get<int>("t");
  try {
	  addF(src.data, dest.data, src.cols, src.rows, threshold,40);
    //processor.Threshold(src.data, src.cols, src.rows, threshold);
  } catch (const std::exception& ex) {
    cout << ex.what() << endl;
    return 0;
  }

  // Show destination image.
  const string kDstWindowName = "Destination image";
  namedWindow(kDstWindowName, WINDOW_NORMAL);
  resizeWindow(kDstWindowName, 640, 480);
  imshow(kDstWindowName, dest);
  waitKey();

  return 0;
}
