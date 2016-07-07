#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "image_processing.hpp"
#include "tracking.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
"This is an empty application that can be treated as a template for your "
"own doing-something-cool applications.";

const char* kOptions =
"{ v video        |        | video to process         }"
"{ h ? help usage |        | print help message       }";

Mat image;
Mat source;
Mat frame;

bool backprojMode = false;
bool selectObject = false;
int trackObject = 0;
bool showHist = true;
Point origin;
Rect selection;
string kSrcWindowName = "ChooseTheObjectToTracking";
cv::VideoCapture vc;
MedianFlowTracker tr;

void StartTracking()
{
	while (1) {
		//cout << "ee";

		vc >> frame;
		//imshow(kSrcWindowName, frame);
		if (frame.empty()) {
			break;
		}
		selection = tr.Track(frame);
		frame.copyTo(image);
		rectangle(image, selection, Scalar(115, 115, 115));
		imshow(kSrcWindowName, image);
		//tr.Init(frame, selection);
		char c = cvWaitKey(33);
		if (c == 27) { // åñëè íàæàòà ESC - âûõîäèì
			break;
		}
	}
}
bool EndSelection = false;

static void onMouse(int event, int x, int y, int, void*)
{
	if (EndSelection)
		return;
	if (selectObject)
	{
		frame.copyTo(image);
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = std::abs(x - origin.x);
		selection.height = std::abs(y - origin.y);
		selection &= Rect(0, 0, image.cols, image.rows);
		rectangle(image, selection, Scalar(115, 115, 115));
		imshow(kSrcWindowName,image );
	}
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		origin = Point(x, y);
		selection = Rect(x, y, 0, 0);
		selectObject = true;
		break;
	case EVENT_LBUTTONUP:
		selectObject = false;
		if (selection.width > 0 && selection.height > 0)
			trackObject = -1;
		//cout << selection.height << endl;
		source.copyTo(image);
		EndSelection = true;
		tr.Init(frame, selection);
		StartTracking();
		//printNewImage();
		break;
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
	string VideoString = parser.get<string>("video");
	cout << VideoString;
	if (VideoString.empty())
	{
		cout << "No such video!!!";
		return 1;
	}
	

	vc.open(VideoString);
	
	vc >> frame;

	namedWindow(kSrcWindowName, WINDOW_NORMAL);
	resizeWindow(kSrcWindowName, 640, 480);
	imshow(kSrcWindowName, frame);
	setMouseCallback(kSrcWindowName, onMouse, 0);
	const int kWaitKeyDelay = 0;
	waitKey(kWaitKeyDelay);
	


	// Do something cool.
	//cout << "This is empty template sample." << endl;

	return 0;
}