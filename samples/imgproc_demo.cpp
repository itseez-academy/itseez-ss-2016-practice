#include <iostream>
#include <string>

#include "image_processing.hpp"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

bool gray = false;
bool median = false;
bool edges = false;
bool pixel = false;
Mat src;

void processImage(const cv::Mat &src, const cv::Rect &roi)
{
	ImageProcessorImpl ipi;
	Mat newMat;

	if (gray)
	{
		newMat = ipi.CvtColor(src, roi);
	}
	else if (median)
	{
		newMat = ipi.Filter(src, roi, 5);
	}
	else if (edges)
	{
		newMat = ipi.DetectEdges(src, roi, 5, 1, 1, 3);
	}
	else if (pixel)
	{
		newMat = ipi.Pixelize(src, roi, 3);
	}
	else
		return;

	// Show destination image.
	const string kDstWindowName = "Processed image";
	namedWindow(kDstWindowName, WINDOW_NORMAL);
	resizeWindow(kDstWindowName, src.cols, src.rows);
	imshow(kDstWindowName, newMat);
	waitKey();
}


struct MouseCallbackState {
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
};

MouseCallbackState mcState;



void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		mcState.is_selection_started = true;
		mcState.point_first = Point(x, y);
		waitKey(40000);
	}
	else if (event == EVENT_MOUSEMOVE)
	{
		//TODO: draw rectangle if selection started
	}
	else if (event == EVENT_LBUTTONUP)
	{
		mcState.is_selection_finished = true;
		mcState.point_second = Point(x, y);

		processImage(src, Rect(mcState.point_first, mcState.point_second));
	}
}


const char* kAbout =
    "Practice2";

const char* kOptions =
"{ @image         | <none> | image to process            }"
"{ gray           |        | convert ROI to gray scale   }"
"{ median         |        | apply median filter for ROI }"
"{ edges          |        | detect edges in ROI         }"
"{ pix            |        | pixelize ROI                }"
"{ h ? help usage |        | print help message          }";


int main(int argc, const char** argv) {
  // Parse command line arguments.
	CommandLineParser parser = CommandLineParser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  mcState.is_selection_started = false;
  mcState.is_selection_finished = false;

  // Read image.
  src = imread(parser.get<string>(0));
  if (src.empty()) {
	  cout << "Failed to open image file '" + parser.get<string>(0) + "'."
		  << endl;
	  return 0;
  }

  if (parser.get<bool>("gray"))
  {
	  gray = true;
  }
  else if (parser.get<bool>("median"))
  {
	  median = true;
  }
  else if (parser.get<bool>("edges"))
  {
	  edges = true;
  }
  else if (parser.get<bool>("pix"))
  {
	  pixel = true;
  }

  // Show source image.
  const string kSrcWindowName = "Source image";
  const int kWaitKeyDelay = 1;
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  resizeWindow(kSrcWindowName, src.cols, src.rows);
  imshow(kSrcWindowName, src);
  waitKey(kWaitKeyDelay);

  setMouseCallback("Source image", CallBackFunc);
  waitKey(40000);


  return 0;
}
