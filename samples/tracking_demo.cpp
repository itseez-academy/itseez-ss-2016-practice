#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

const char* kAbout = "Practice 4";

const char* kOptions =
	"{ v video        | <none> | video to process         }"
	"{ c camera       | <none> | camera to get video from }"
    "{ h ? help usage |        | print help message       }";

const string windowName = "Tracking demo";

int main(int argc, const char** argv) 
{
	// Parse command line arguments.
	CommandLineParser parser(argc, argv, kOptions);
	parser.about(kAbout);

	// If help option is given, print help message and exit.
	if (parser.get<bool>("help")) 
	{
		parser.printMessage();
		return 0;
	}

	namedWindow(windowName, WINDOW_NORMAL);
	resizeWindow(windowName, 640, 480);

	//TODO: load video etc

	if (parser.has("video") || parser.has("camera"))
	{
		VideoCapture cap;

		if (parser.has("video"))
			cap = VideoCapture(parser.get<string>("video"));
		else
			cap = VideoCapture(0);

		if (!cap.isOpened())
		{
			cout << "Failed to open video capture." << endl;
			return 1;
		}

		for (;;)
		{
			Mat frame;
			cap >> frame;

			if (frame.empty()) break;

			imshow(windowName, frame);

			if (cv::waitKey(30) >= 0) break;
		}
	}

	return 0;
}
