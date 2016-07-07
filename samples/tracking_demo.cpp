#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

const char* kAbout = "Practice 4";

const char* kOptions =
	"{ v video        | <none> | video to process         }"
	"{ c camera       | <none> | camera to get video from }"
    "{ h ? help usage |        | print help message       }";


struct MouseCallbackState 
{
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
};

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	MouseCallbackState * mcState = (MouseCallbackState *)userdata;

	if (event == EVENT_LBUTTONDOWN)
	{
		if (!mcState->is_selection_started)
		{
			mcState->is_selection_started = true;
			mcState->point_first = Point(x, y);
		}
	}
	else if (event == EVENT_MOUSEMOVE)
	{
		if (mcState->is_selection_started && !mcState->is_selection_finished)
		{
			mcState->point_second = Point(x, y);
		}
	}
	else if (event == EVENT_LBUTTONUP && !mcState->is_selection_finished)
	{
		mcState->is_selection_finished = true;
		mcState->point_second = Point(x, y);
	}
}

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

	waitKey(1);
	MouseCallbackState * mcState = new MouseCallbackState(); //it will be deleted when the application is closed
	mcState->is_selection_started = false;
	mcState->is_selection_finished = false;
	setMouseCallback(windowName, CallBackFunc, mcState);

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

			if (mcState->is_selection_finished)
			{
				imshow(windowName, frame);
				//TODO: track and show tracking results
				//Rect(mcState->point_first, mcState->point_second);
			}
			else if (mcState->is_selection_started)
			{
				Mat shownPicture = frame.clone();
				rectangle(shownPicture, mcState->point_first, mcState->point_second, Scalar(0, 0, 0));
				imshow(windowName, shownPicture);
			}
			else
			{
				imshow(windowName, frame);
			}

			if (cv::waitKey(30) >= 0) break;
		}
	}

	return 0;
}
