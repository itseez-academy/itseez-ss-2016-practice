#include <iostream>
#include <string>
#include"tracking.hpp"
#include "opencv2/core.hpp"
#include"opencv2/highgui.hpp"
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

static void OnMouse(int event, int x, int y, int, void* _state) {
	MouseCallbackState* state = static_cast<MouseCallbackState*>(_state);
	switch (event) {
	case cv::EVENT_LBUTTONDOWN:
		state->is_selection_started = true;
		state->is_selection_finished = false;
		state->point_first = Point(x, y);
		break;
	case cv::EVENT_LBUTTONUP:
		state->is_selection_finished = true;
		state->is_selection_started = false;
		break;
	case cv::EVENT_MOUSEMOVE:
		if (state->is_selection_started && !state->is_selection_finished) {
			state->point_second = Point(x, y);
		}
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
  namedWindow("win");
  MouseCallbackState mouse_state;
  mouse_state.is_selection_started = false;
  mouse_state.is_selection_finished = false;
 // setMouseCallback("win", OnMouse, &mouse_state);
  VideoCapture capture(0);
  Mat frame;
  Rect roi;
 // while( frame.empty())capture >> frame;
 // capture >> frame;
 // imshow("win", frame);
//  while (!mouse_state.is_selection_finished) {
//	  waitKey(100);
//  }
 // roi = Rect(mouse_state.point_first, mouse_state.point_second);
  roi = Rect(50, 50, 190, 190);
  capture >> frame;
  MedianFlowTracker tracker;
  tracker.Init(frame, roi);
  
  for (;;)
  {
	  
	  roi = tracker.Track(frame);
	  rectangle(frame, roi, Scalar(0, 0, 255));
	  if (frame.empty())break;
	  imshow("win",frame);
	  if (cv::waitKey(200) == 27)
		  break;
	  capture >> frame;
  
  }
  // Do something cool.
 

  return 0;
}
