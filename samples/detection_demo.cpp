#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include <opencv2/imgproc/imgproc.hpp>

#include "opencv2/highgui.hpp"
#include "detection.hpp"

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
	CascadeDetector detector;
	Mat img = imread(parser.get<string>("i"));
	if (img.empty()) {
		cout << "Failed to open image file '" + parser.get<string>(0) + "'."
			<< endl;
		return 0;
	}
	detector.Init(parser.get<string>("m"));
	vector<Rect> objects;
	vector<int> scores;
	detector.Detect(img, objects, scores);

	// Show source image.
	const string kSrcWindowName = "Image";
	const string kDstWindowName = "Image2";
	const int kWaitKeyDelay = 1;
	namedWindow(kSrcWindowName, WINDOW_NORMAL);
	resizeWindow(kSrcWindowName, 640, 480);
	for (int i = 0; i < objects.size(); i++)
	{
		
		rectangle(img, objects.at(i), Scalar(8,128,16), 1, 8, 0);
			

	}
	
	imshow(kSrcWindowName, img);
	waitKey(0);
	cout << "This is empty template sample." << endl;

	return 0;
}
