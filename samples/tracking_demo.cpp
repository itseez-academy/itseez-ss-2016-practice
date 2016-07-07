#include <iostream>
#include <string>

#include "tracking.hpp"

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

Rect getContainedRoi(const Mat & src, const Rect & roi)
{
	Rect newRoi = Rect();

	newRoi.x = max(0, roi.x);
	newRoi.y = max(0, roi.y);

	newRoi.width = roi.x < 0 ? roi.width + roi.x : roi.width;
	newRoi.height = roi.y < 0 ? roi.height + roi.y : roi.height;

	newRoi.width = min(newRoi.width, src.cols - newRoi.x);
	newRoi.height = min(newRoi.height, src.rows - newRoi.y);

	return newRoi;
}

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

void showWithRect(const Mat & frame, const Rect & rect)
{
	Mat shownPicture = frame.clone();
	rectangle(shownPicture, rect, Scalar(0, 0, 0));
	imshow(windowName, shownPicture);
}

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

		std::shared_ptr<Tracker> tracker = Tracker::CreateTracker("median_flow");
		bool trackingInitialized = false;

		for (;;)
		{
			Mat frame;
			cap >> frame;

			if (frame.empty()) break;

			if (mcState->is_selection_finished)
			{
				if (!trackingInitialized)
				{
					Rect actualRoi = getContainedRoi(frame, Rect(mcState->point_first, mcState->point_second));
					
					if (!tracker->Init(frame, actualRoi))
					{
						cout << "Failed to initialize tracker";
						return 1;
					}

					showWithRect(frame, actualRoi);

					trackingInitialized = true;
				}
				else
				{
					showWithRect(frame, tracker->Track(frame));
				}
			}
			else if (mcState->is_selection_started)
				showWithRect(frame, Rect(mcState->point_first, mcState->point_second));
			else
				imshow(windowName, frame);

			if (cv::waitKey(30) >= 0) break;
		}
	}

	return 0;
}
