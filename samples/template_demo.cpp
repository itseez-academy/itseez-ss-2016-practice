#include <iostream>
#include <string>

#include "opencv2/opencv.hpp"
#include "image_processing.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
    "{ @image         |        | image                    }"
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

	// Show source image.
	const string kSrcWindowName = "Source image";
	const int kWaitKeyDelay = 1;
	namedWindow(kSrcWindowName, WINDOW_NORMAL);
	resizeWindow(kSrcWindowName, 640, 480);
	imshow(kSrcWindowName, src);
	waitKey(kWaitKeyDelay);

	ImageProcessorImpl imgproc;
	//imgproc.CvtColor(src, Rect(5, 5, 50, 50));

	// Show destination image.
	/*const string kDstWindowName = "Destination image";
	namedWindow(kDstWindowName, WINDOW_NORMAL);
	resizeWindow(kDstWindowName, 640, 480);
	imshow(kDstWindowName, imgproc.CvtColor(src, Rect(5, 5, 250, 250)));*/

	/*const string kDstWindowName2 = "Destination image 2";
	namedWindow(kDstWindowName2, WINDOW_NORMAL);
	resizeWindow(kDstWindowName2, 640, 480);
	imshow(kDstWindowName2, imgproc.Filter(src, Rect(5, 5, 250, 250),1));
	*/
	const string kDstWindowName3 = "Destination image 3";
	namedWindow(kDstWindowName3, WINDOW_NORMAL);
	resizeWindow(kDstWindowName3, 640, 480);
	imshow(kDstWindowName3, imgproc.DetectEdges(src, Rect(5, 5, 250, 250), 5, 50, 3,5));
	waitKey();
  return 0;
}
