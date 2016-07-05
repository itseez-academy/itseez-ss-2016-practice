#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "image_processing.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
				"{ @image         | <none> | image to process            }"
				"{ gray           |        | convert ROI to gray scale   }"
				"{ median         |        | apply median filter for ROI }"
				"{ edges          |        | detect edges in ROI         }"
				"{ pix            |        | pixelize ROI                }"
				"{ h ? help usage |        | print help message          }";

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
  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  }

  Mat src = imread(parser.get<string>(0), CV_LOAD_IMAGE_COLOR);
  if (src.empty()) {
	  cout << "Failed to open image file '" + parser.get<string>(0) + "'."
		  << endl;
	  return 0;
  }

  // Show source image.
  const string kSrcWindowName = "Source image";
  const int kWaitKeyDelay = 1;
  namedWindow(kSrcWindowName);
  imshow(kSrcWindowName, src);
  waitKey(kWaitKeyDelay);

  MouseCallbackState mouse_state;
  mouse_state.is_selection_finished = false;
  setMouseCallback(kSrcWindowName, OnMouse, &mouse_state);

  for (;;)
  {
	  if (mouse_state.is_selection_finished)
	  {
		  int x = mouse_state.point_first.x;
		  int y = mouse_state.point_first.y;
		  int x2 = mouse_state.point_second.x;
		  int y2 = mouse_state.point_second.y;

		  Rect roi;
		  if (x2 > x && y2 > y)
			  roi = Rect(x, y, x2 - x, y2 - y);
		  else
			  roi = Rect(100, 100, 300, 300);

		  Mat dsr;

		  MyImageProcessor processor;

		  try {
			  dsr = processor.CvtColor(src, roi);
		  }
		  catch (const std::exception& ex) {
			  cout << ex.what() << endl;
			  return 0;
		  }

		  const string kDstWindowName = "Destination image";
		  namedWindow(kDstWindowName);
		  imshow(kDstWindowName, dsr);
	 }
	  waitKey(kWaitKeyDelay);
  }
  
  waitKey();

  return 0;
}
