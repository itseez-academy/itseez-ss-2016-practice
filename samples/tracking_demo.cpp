#include <iostream>
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include"opencv2\opencv.hpp"
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
	
	if (event == EVENT_LBUTTONDOWN)
	{
		((MouseCallbackState*)userdata)->is_selection_started = true;
		((MouseCallbackState*)userdata)->is_selection_finished = false;
		((MouseCallbackState*)userdata)->point_first = Point(x, y);
		cout << "Left button of the mouse is down - position (" << x << ", " << y << ")" << endl;
	}
	if (event == EVENT_LBUTTONUP)
	{
		((MouseCallbackState*)userdata)->is_selection_started = false;
		((MouseCallbackState*)userdata)->is_selection_finished = true;
		((MouseCallbackState*)userdata)->point_second = Point(x, y);
		cout << "Left button of the mouse is up - position (" << x << ", " << y << ")" << endl;
	}
	if (event == EVENT_MOUSEMOVE && ((MouseCallbackState*)userdata)->is_selection_finished == false)
	{
		((MouseCallbackState*)userdata)->point_second = Point(x, y);
	}
}
void toGrey(cv::Mat & grfr) {
	cvtColor(grfr, grfr, CV_BGR2GRAY);
	grfr.convertTo(grfr, CV_8UC1);

};
int main(int argc, const char** argv) {
 
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);
  MouseCallbackState *MouseClick = new MouseCallbackState();
  Mat img;
  VideoCapture cap("C:/Temp/proj1/build/bin/1.mpg");
  cap >> img;
  if (img.empty())
  {
	  return -1;
  }
  
  MedianFlowTracker mft;
  Mat grfr;
 
  cv::namedWindow("window1", 1);
  setMouseCallback("window1", OnMouse, (void*)MouseClick);
  cv::imshow("window1", img);
  cv::waitKey(30);

  while (!MouseClick->is_selection_finished) {
	  waitKey(25);
  }
  Rect roi(MouseClick->point_first, MouseClick->point_second);

  img.copyTo(grfr);
  
  toGrey(grfr);
  
  mft.Init(grfr, roi);

  cv::rectangle(img, roi, Scalar(0, 0, 0, 0));
  imshow("window1", img);
  waitKey(0);
  Rect r;
  for (;;) {
	  cap >> img;
	  if (img.empty())
	  {
		  cout << "Error loading the image" << endl;
		  return -1;
	  }
	  
	  img.copyTo(grfr);
	  toGrey(grfr);

	  r=mft.Track(grfr);
	  std::cout << "dx= " << r.x << "      " << "dy= " << r.y << std::endl;
	  mft.Init(grfr,r);
	  cv::rectangle(img, r, Scalar(0, 0, 0, 0));
	  imshow("window1", img);
	  waitKey(25);
	  if (waitKey(30) >= 0) break;

  }

  cv::waitKey(0);
  return 0;
}
