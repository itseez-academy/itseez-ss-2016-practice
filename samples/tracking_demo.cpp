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
    "{ v video        |        | video to process         }"
    "{ h ? help usage |        | print help message       }";

struct MouseCallbackState {
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;

	MouseCallbackState() : is_selection_started(false), is_selection_finished(false) {}
};

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	MouseCallbackState * p_mc_state = (MouseCallbackState *)userdata;
	if (event == EVENT_LBUTTONDOWN)
	{
		p_mc_state->point_first = Point(x, y);
		p_mc_state->is_selection_started = true;
	}
	else if (event == EVENT_LBUTTONUP)
	{
		p_mc_state->is_selection_finished = true;
	}
	p_mc_state->point_second = Point(x, y);
}

int main(int argc, const char** argv) {
	// Parse command line arguments.
	CommandLineParser parser(argc, argv, kOptions);
	parser.about(kAbout);

	// If help option is given, print help message and exit.
	if (parser.get<bool>("help")) {
		parser.printMessage();
		return 0;
	}

	if (parser.has("video")) {
		string vfilepath = parser.get<string>("video");
		VideoCapture cap(vfilepath);
		Mat frame;
		if (!cap.isOpened()) {
			cerr << "Can't open video capture!" << parser.get<string>("video") << endl;
			return -1;
		}
		cap >> frame;
	
		const string kSrcWindowName = "Tracking object";
		imshow(kSrcWindowName, frame);
		MouseCallbackState mc_state;
		setMouseCallback(kSrcWindowName, CallBackFunc, (void *)&mc_state);
		Rect roi;

		while (!mc_state.is_selection_finished) {
			if (mc_state.is_selection_started) {
				Mat frame_copy = frame.clone();
				roi = Rect(mc_state.point_first, mc_state.point_second);
				rectangle(frame_copy, roi, Scalar(255, 0, 0));
				imshow(kSrcWindowName, frame_copy);
			}
			waitKey(30);
		}

		MedianFlowTracker tracker;
		tracker.Init(frame, roi);
		while (frame.cols * frame.rows)
		{
			Rect obj = tracker.Track(frame);
			Mat tracking_frame = frame.clone();
			rectangle(tracking_frame, obj, Scalar(255, 0, 0));
			imshow("Tracking object", tracking_frame);
			waitKey(30);
			cap >> frame;
		}
	}

	return 0;
}
