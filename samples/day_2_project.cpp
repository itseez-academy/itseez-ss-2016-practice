#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "image_processing.hpp"
#include <windows.h>
using namespace std;
using namespace cv;

const char* kAbout = "Application for practice #2.";

const char* kOptions =
"{ @image         |        | image to process            }"
"{ gray           | <none> | convert ROI to gray scale   }"
"{ median         | <none> | apply median filter for ROI }"
"{ edges          | <none> | detect edges in ROI         }"
"{ pix            | <none> | pixelize ROI                }"
"{ h ? help usage |        | print help message          }";

struct MouseCallbackState {
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
};

void OnMouse(int event, int x, int y, int flags, void* userdata) {
	MouseCallbackState* mouse = reinterpret_cast<MouseCallbackState*>(userdata);
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		mouse->is_selection_started = true;
		mouse->is_selection_finished = false;
		mouse->point_first.x = x;
		mouse->point_first.y = y;
	
		//GetCursorPos(&mouse.point_first);


	}
	if (event == cv::EVENT_LBUTTONUP)
	{
		mouse->is_selection_started = false;
		mouse->is_selection_finished = true;
		//GetCursorPos(&mouse.point_second);
		mouse->point_second.x = x;
		mouse->point_second.y = y;
	
	}
	if (event == cv::EVENT_MOUSEMOVE&&mouse->is_selection_finished == false)
	{
		//GetCursorPos(&mouse.point_second);
		mouse->point_second.x = x;
		mouse->point_second.y = y;
	
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

  // Read image.
  Mat src = imread(parser.get<string>(0));
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
  setMouseCallback(kSrcWindowName, OnMouse, &mouse_state);

  while (!mouse_state.is_selection_finished) {
	  waitKey(1);
  }
  Rect roi(mouse_state.point_first, mouse_state.point_second);

  // Threshold data.
  MyImageProc ImProc;
  if (parser.has("gray")) {

	  // Show destination image.
	 const string kDstWindowName = "Destination image";
	 namedWindow(kDstWindowName);
	  imshow(kDstWindowName, ImProc.CvtColor(src, roi));
	  waitKey();

    
  }

  if( parser.has("median"))
  {
	  // Show destination image.
	  const string kDstWindowName = "Destination image";
	  namedWindow(kDstWindowName);
	  imshow(kDstWindowName, ImProc.Filter(src, roi, 15));
	  waitKey();
   
  }

  if( parser.has("edges"))
  {
	  // Show destination image.
	  //const string kDstWindowName = "Destination image";
	  //namedWindow(kDstWindowName);
	  //imshow(kDstWindowName, ImProc.DetectEdges(src, roi, 4, 4, 4, 4));
	  //waitKey();

	  
    }

  if( parser.has("pix"))
   {
	  ImProc.Pixelize(src, roi,	 10);
    }


  return 0;
}
