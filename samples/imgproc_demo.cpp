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
};

struct ProgState {
	MouseCallbackState mouseState;
	bool GrayOk;
};

void OnMouse(int event, int x, int y, int flags, void* params )
{
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

	

	const string kSrcWindowName = "Source image";
	
	const int kWaitKeyDelay = 1;
	namedWindow(kSrcWindowName, WINDOW_NORMAL);
	resizeWindow(kSrcWindowName, 640, 480);
	ImageProcessorImpl processor;
	MouseCallbackState state;
	setMouseCallback(kSrcWindowName, OnMouse, &state);
	imshow(kSrcWindowName, img);

	for (;;)
	{
		if (state.is_selection_finished)
		{
			img = processor.CvtColor(img, Rect(state.point_second, state.point_first));
			imshow(kSrcWindowName, img);
		}

		char c = waitKey(30);
		if (c == 27) { // если нажата ESC - выходим
			break;
		}
	}
	return 0;
}


