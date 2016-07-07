#include <iostream>
#include <string>

#include "opencv2/core.hpp"
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
};

void OnMouse(int event, int x, int y, int flags, void* userdata)
{
	MouseCallbackState* state = reinterpret_cast<MouseCallbackState*>(userdata);
	if (event == EVENT_LBUTTONDOWN)
	{
		state->is_selection_started = true;
		state->is_selection_finished = false;
		state->point_first = Point(x, y);
	}
	if (event == EVENT_LBUTTONUP)
	{
		state->is_selection_started = false;
		state->is_selection_finished = true;
		state->point_second = Point(x, y);
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

  string pathToFile;
  if (parser.has("v"))
	  pathToFile = parser.get<string>("v");
  else
	  return -1;

  VideoCapture cap;
  cap.open(pathToFile);

  if (!cap.isOpened()) {
	  cout << "Fail." << endl;
	  return -1;
  }

  Mat img;
  cap >> img; if (img.empty()) return 0;
  const string kSrcWindowName = "Tracking video";
  imshow(kSrcWindowName, img);

  MouseCallbackState mouse_state;
  mouse_state.is_selection_finished = false;
  setMouseCallback(kSrcWindowName, OnMouse, &mouse_state);
  Rect roi;

  for (;;)
  {
	  if (mouse_state.is_selection_finished)
	  {
		  int x = mouse_state.point_first.x;
		  int y = mouse_state.point_first.y;
		  int x2 = mouse_state.point_second.x;
		  int y2 = mouse_state.point_second.y;
		  
		  if (x2 > x && y2 > y)
			  roi = Rect(x, y, x2 - x, y2 - y);

		  rectangle(img, roi, Scalar(0, 255, 255));
		  imshow(kSrcWindowName, img);

		  break;
	  }
	  waitKey(5);
  }

  shared_ptr<MedianFlowTracker> tracker = MedianFlowTracker::CreateTracker("median_flow");
  Mat img_gray;
  cvtColor(img, img_gray, CV_BGR2GRAY);
  if (!tracker->Init(img_gray, roi))
	  return 0;

  for (;;)
  {
	  cap >> img; if (img.empty()) break;
	  cvtColor(img, img_gray, CV_BGR2GRAY);
	  Rect roi = tracker->Track(img_gray);

	  rectangle(img, roi, Scalar(0, 255, 255));
	  imshow(kSrcWindowName, img);
	  waitKey(10);
  }

  waitKey(0);

  return 0;
}
