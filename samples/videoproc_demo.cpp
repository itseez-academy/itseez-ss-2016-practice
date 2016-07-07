#include <iostream>
#include <string>

#include "image_processing.hpp"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

bool gray = false;
bool median = false;
bool edges = false;
bool pixel = false;

Mat src;

const string beforeProcessing = "Before processing";
const string afterProcessing = "After processing";

void createNamedWindows(int width, int height)
{
	string name = beforeProcessing;
	namedWindow(name, WINDOW_NORMAL);
	resizeWindow(name, width, height);

	name = afterProcessing;
	namedWindow(name, WINDOW_NORMAL);
	resizeWindow(name, width, height);
}

void processImage(const cv::Rect &roi)
{
	ImageProcessorImpl ipi;
	Mat newMat;

	if (gray)
		newMat = ipi.CvtColor(src, roi);
	else if (median)
		newMat = ipi.Filter(src, roi, 5);
	else if (edges)
		newMat = ipi.DetectEdges(src, roi, 5, 1, 1, 3);
	else if (pixel)
		newMat = ipi.Pixelize(src, roi, 3);
	else
		return;

	imshow(afterProcessing, newMat);
}


struct MouseCallbackState {
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
};


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	MouseCallbackState * mcState = (MouseCallbackState *)userdata;

	if (event == EVENT_LBUTTONDOWN)
	{
		if (!mcState->is_selection_started)
		{
			mcState->is_selection_started = true;
			mcState->point_first = Point(x, y);
		}
	}
	else if (event == EVENT_MOUSEMOVE)
	{
		if (mcState->is_selection_started && !mcState->is_selection_finished)
		{
			mcState->point_second = Point(x, y);
		}
	}
	else if (event == EVENT_LBUTTONUP && !mcState->is_selection_finished)
	{
		mcState->is_selection_finished = true;
		mcState->point_second = Point(x, y);
	}
}


const char* kAbout =
    "Practice2 extras";

const char* kOptions =
"{ @video         | <none> | type 'cam' to use camera    }"
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

  MouseCallbackState * mcState = new MouseCallbackState(); //it will be deleted when the application is closed

  mcState->is_selection_started = false;
  mcState->is_selection_finished = false;

  string arg = parser.get<string>(0);
  VideoCapture cap;

  if (arg.compare("cam") == 0)
  {
	  cap = VideoCapture(0);
  }
  else
  {
	  cap = VideoCapture(arg);
  }

  if (!cap.isOpened())
  {
	  cout << "Failed to open video capture." << endl;
	  return 1;
  }

  if (parser.get<bool>("gray"))
	  gray = true;
  else if (parser.get<bool>("median"))
	  median = true;
  else if (parser.get<bool>("edges"))
	  edges = true;
  else if (parser.get<bool>("pix"))
	  pixel = true;

  createNamedWindows(640, 480);

  waitKey(1);

  setMouseCallback(beforeProcessing, CallBackFunc, mcState);

  for (;;)
  {
	  cap >> src;

	  if (src.empty()) break;

	  if (mcState->is_selection_finished)
	  {
		  imshow(beforeProcessing, src);
		  processImage(Rect(mcState->point_first, mcState->point_second));
	  }
	  else if (mcState->is_selection_started)
	  {
		  Mat shownPicture = src.clone();
		  rectangle(shownPicture, mcState->point_first, mcState->point_second, Scalar(0, 0, 0));
		  imshow(beforeProcessing, shownPicture);
	  }
	  else
	  {
		  imshow(beforeProcessing, src);
	  }

	  if (cv::waitKey(30) >= 0) break;
  }

  return 0;
}