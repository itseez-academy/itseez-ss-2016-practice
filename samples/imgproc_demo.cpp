#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <image_processing.hpp>

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
	ImageProcessorImpl processor;
	Mat img;
	Rect roi;
	bool grayOk;
	string kSrcWindowName;
};

struct ProgState {
	MouseCallbackState mouseState;
	bool GrayOk;
};

void OnMouse(int event, int x, int y, int flags, void* params )
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
			
			state->img = state->processor.CvtColor(state->img, Rect (state->point_second, state->point_first));
			imshow(kSrcWindowName, state->img);
		}
	else if (event == EVENT_RBUTTONDOWN)
	{
		//cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MBUTTONDOWN)
	{
		//cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MOUSEMOVE)
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

	// Do something cool.
	cout << "This is empty template sample." << endl;

	// Read image.
	Mat img = imread(parser.get<string>(0));
	if (img.empty()) {
		cout << "Failed to open image file '" + parser.get<string>(0) + "'."
			<< endl;
		return 0;
	}

	

	const string kSrcWindowName = "Source image";
	
	const int kWaitKeyDelay = 1;
	namedWindow(kSrcWindowName, WINDOW_NORMAL);
	resizeWindow(kSrcWindowName, 640, 480);
	ImageProcessorImpl processor;
	MouseCallbackState state;
	state.img = img;
	state.processor = processor;
	state.kSrcWindowName = kSrcWindowName;
	setMouseCallback(kSrcWindowName, OnMouse, &state);
	imshow(kSrcWindowName, img);
	bool grayOk = false;
	const int threshold = parser.get<int>("gray");
	try {
		grayOk = true;
		
	}
	
	catch (const std::exception& ex) {
		cout << ex.what() << endl;
		return 0;
	}
	state.grayOk = grayOk;
	
	imshow(kSrcWindowName, state.img);
	/*
	const string kDstWindowName = "Destination image";
	namedWindow(kDstWindowName, WINDOW_NORMAL);
	resizeWindow(kDstWindowName, 640, 480);
	setMouseCallback(kDstWindowName, DstImgCallback, &state);

	if (grayOk) {

	}*/
	
	
	//waitKey(kWaitKeyDelay);
	waitKey(0);

	//Create a window
	//namedWindow("My Window", 1);

	//set the callback function for any mouse event
	//setMouseCallback("My Window", CallBackFunc, NULL);

	//show the image
	//imshow("My Window", img);

	// Wait until user press some key
	//waitKey(0);

	return 0;
}


