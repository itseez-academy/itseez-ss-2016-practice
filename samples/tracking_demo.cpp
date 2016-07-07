#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <image_processing.hpp>
#include <tracking.hpp>

using namespace std;
using namespace cv;

const char* kAbout =
"This is an empty application that can be treated as a template for your "
"own doing-something-cool applications.";

const char* kOptions =
"{ @image         |        | image to process         }"
"{ v video        |        | video to process         }"
"{ gray           |        | convert ROI to gray scale}"
"{ h ? help usage |        | print help message       }";


struct MouseCallbackState {
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
};


void OnMouse(int event, int x, int y, int flags, void* params)
{

	//state.
	MouseCallbackState* state = (MouseCallbackState*)params;
	if (event == EVENT_LBUTTONDOWN)
	{
		state->is_selection_started = true;
		state->is_selection_finished = false;
		state->point_first.x = x;
		state->point_first.y = y;
	}


	if (event == EVENT_LBUTTONUP)
	{
		state->is_selection_started = false;
		state->is_selection_finished = true;
		state->point_second.x = x;
		state->point_second.y = y;
	}
	if (event == EVENT_RBUTTONDOWN)
	{
		//cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	if (event == EVENT_MBUTTONDOWN)
	{
		//cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	if (event == EVENT_MOUSEMOVE)
	{
		if (state->is_selection_finished == false)
		{
			state->point_second.x = x;
			state->point_second.y = y;
		}

	}
}


void DstImgCallback(int event, int x, int y, int flags, void* params)
{

	//state.
	MouseCallbackState* state = (MouseCallbackState*)params;
	if (event == EVENT_LBUTTONDOWN)
	{
		state->is_selection_started = true;
		state->is_selection_finished = false;
		state->point_first.x = x;
		state->point_first.y = y;
	}

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

	bool grayOk = false;
	if (parser.has("gray"))
		grayOk = true;

	// Do something cool.
	cout << "This is empty template sample." << endl;

	// Read image.
	Mat img = imread(parser.get<string>(0));
	if (img.empty()) {
		cout << "Failed to open image file '" + parser.get<string>(0) + "'."
			<< endl;
		return 0;
	}

	VideoCapture videoCap("F:/GTAtitles.mpg"); // open the video file for reading
	

	//if (!videoCap.isOpened())  // if not success, exit program
	//{
	//	cout << "Cannot open the video file" << endl;
	//	return -1;
	//}
	


	const string kSrcWindowName = "Source image";

	const int kWaitKeyDelay = 1;
	namedWindow(kSrcWindowName, WINDOW_NORMAL);
	resizeWindow(kSrcWindowName, 640, 480);
	MedianFlowTracker tracker;
	ImageProcessorImpl processor;
	MouseCallbackState state;
	state.is_selection_finished = true;
	state.is_selection_started = true;
	setMouseCallback(kSrcWindowName, OnMouse, &state);
	bool detectChanges = state.is_selection_finished;
	char c;
	Mat frame;
	bool bSuccess = true;// = videoCap.read(frame); // read a new frame from video

	videoCap >> frame;
	if (frame.empty())
	{
		return -1;
	}

	//if (!bSuccess) //if not success, break loop
	//{
	//	cout << "Cannot read the frame from video file" << endl;
	//	return 0;
	//}
	imshow(kSrcWindowName, frame);
	for (;;)
	{
		
		videoCap >> frame;
		//bSuccess = videoCap.read(frame); // read a new frame from video

		/*if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}*/
		
		//img = tracker.Init(img, Rect(state.point_second, state.point_first));
		if ((detectChanges != state.is_selection_finished) & (state.is_selection_finished))
		{
			frame = tracker.Init(frame, Rect(state.point_second, state.point_first));
			rectangle(frame, Rect(state.point_second, state.point_first), Scalar(0, 128, 0), 2, 8, 0);
			imshow(kSrcWindowName, frame);
			
		}
		detectChanges = state.is_selection_finished;
		
		if (!state.is_selection_started)
		{
			tracker.Track(frame);
		}

		c = waitKey(30);
		if (c == 27) { // если нажата ESC - выходим
			break;
		}
	}
	return 0;
}


