#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "tracking.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
"{ v video        | <none> | video to process                         }"
"{ c camera       | <none> | camera to get video from                 }"
"{ h ? help usage |        | print help message                       }";

struct MousePosition {
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
} mouse;

static void OnMouse(int event, int x, int y, int, void*) {
	switch (event) {
	case EVENT_LBUTTONDOWN:
		mouse.is_selection_started = true;
		mouse.is_selection_finished = false;
		mouse.point_first = Point(x, y);
		break;
	case EVENT_LBUTTONUP:
		mouse.is_selection_started = true;
		mouse.is_selection_finished = true;
		mouse.point_second = Point(x, y);
		break;
	case EVENT_MOUSEMOVE:
		if (mouse.is_selection_started && !mouse.is_selection_finished)
			mouse.point_second = Point(x, y);
	}
}

int main(int argc, const char** argv) {
	CommandLineParser parser(argc, argv, kOptions);
	parser.about(kAbout);

	if (parser.get<bool>("help")) {
		parser.printMessage();
		return 0;
	}

	MedianFlowTracker tracker;

	if (parser.has("video")) {
		VideoCapture cap(parser.get<string>("video"));
		if (!cap.isOpened())
			cout << "Error! Can't open video" << endl;
		Mat src;
		cap >> src;

		const string kSrcWindowName = "Source image";
		const int kWaitKeyDelay = 1;
		namedWindow(kSrcWindowName, WINDOW_NORMAL);
		setMouseCallback(kSrcWindowName, OnMouse, 0);
		resizeWindow(kSrcWindowName, 640, 480);
		imshow(kSrcWindowName, src);
		waitKey(kWaitKeyDelay);

		mouse.is_selection_started = false;
		mouse.is_selection_finished = false;
		Rect roi;
		while (!mouse.is_selection_finished) {
			if (mouse.is_selection_started) {
				Mat src_cpy;
				src.copyTo(src_cpy);
				roi.x = mouse.point_first.x;
				roi.y = mouse.point_first.y;
				roi.width = mouse.point_second.x - mouse.point_first.x;
				roi.height = mouse.point_second.y - mouse.point_first.y;
				rectangle(src_cpy, roi, Scalar(254));
				imshow(kSrcWindowName, src_cpy);
			}
			waitKey(30);
		}

		for (;;) {
			tracker.Init(src, roi);
			cap >> src;
			if (src.empty())
				break;
			
			roi = tracker.Track(src);
			if (waitKey(30) >= 0)
				break;
			rectangle(src, roi, Scalar(254));
			imshow(kSrcWindowName, src);

		}
	}
	/*
	if (parser.has("camera")) {
		VideoCapture cap(parser.get<int>("camera"));
		if (!cap.isOpened())
			cout << "Error!" << endl;
		for (;;) {
			Mat src;
			cap >> src;
			if (src.empty())
				break;
			detectOnImage(src, detector);
			if (waitKey(30) >= 0)
				break;
		}

	}
	*/
	return 0;
}
