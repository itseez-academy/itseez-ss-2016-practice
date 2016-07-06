#include <iostream>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core.hpp"

using namespace std;
using namespace cv;

const char* kOptions =
"{ @image         | <none> | image to process            }"
"{ gray           |        | convert ROI to gray scale   }"
"{ median         |        | apply median filter for ROI }"
"{ edges          |        | detect edges in ROI         }"
"{ pix            |        | pixelize ROI                }"
"{ h ? help usage |        | print help message          }";



	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;


void OnMouse(int event, int x, int y, int flags)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		is_selection_started = true;
		is_selection_finished = false;
		point_first = Point(x, y);
	}

	if (event == EVENT_LBUTTONUP)
	{
		is_selection_started = false;
		is_selection_finished = true;
		point_second = Point(x, y);
	}

	if (event == EVENT_MOUSEMOVE)
	{
		if (is_selection_finished = false)
			point_second = Point(x, y);
	}
}

const char* kAbout =
"This is an empty application that can be treated as a template for your "
"own doing-something-cool applications.";

const char* kOptions =
"{ v video        |        | video to process         }"
"{ h ? help usage |        | print help message       }";

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

	return 0;
}