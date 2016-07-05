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

//const char* kOptions =
   // "{ v video        |        | video to process         }"
   // "{ h ? help usage |        | print help message       }";
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

static void onMouse(int event, int x, int y, int, void* data)
{
	MouseCallbackState *state = (MouseCallbackState *)data;
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		state->is_selection_started = true;
		state->is_selection_finished = false;
		state->point_first = Point(x, y);
		break;
	case EVENT_LBUTTONUP:
		state->is_selection_started = false;
		state->is_selection_finished = true;
		state->point_second = Point(x, y);
		break;
	case EVENT_MOUSEMOVE:
		state->is_selection_started = true;
		state->is_selection_finished = false;
		state->point_second = Point(x, y);
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
  
  const int kWaitTime = 15;
  Mat src = imread(parser.get<string>(0));
  MouseCallbackState mouseState;
  const string srcWinName = "Source image";
  namedWindow(srcWinName);
  setMouseCallback(srcWinName, onMouse, &mouseState);
  imshow(srcWinName, src);
  waitKey(kWaitTime);

  
  if (parser.get<bool>("gray")) {
	 
	 return 0;
  }
  else if (parser.get<bool>("median")) {
	  parser.printMessage();
	  return 0;
  }

  

  // Do something cool.
  cout << "This is empty template sample." << endl;

  return 0;
}
