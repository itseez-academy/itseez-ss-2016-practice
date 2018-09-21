#include <iostream>
#include <string>
#include <image_processing.hpp>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

//const char* kOptions =
//    "{ v video        |        | video to process         }"
//    "{ h ? help usage |        | print help message       }";

const char* kOptions =
"{ @image         | <none> | image to process            }"
"{ gray           |        | convert ROI to gray scale   }"
"{ median         |        | apply median filter for ROI }"
"{ edges          |        | detect edges in ROI         }"
"{ pix            |        | pixelize ROI                }"
"{ h ? help usage |        | print help message          }";

struct MouseCallbackState{
   bool is_selection_started;
   bool is_selection_finished;
   Point point_first;
   Point point_second;
};
MouseCallbackState mouse;
void OnMouse(int event, int x, int y, int flags, void* param)
{	
	switch (event)
	{
	case EVENT_LBUTTONDOWN:

		mouse.is_selection_started = true;
		mouse.is_selection_finished = false;
		mouse.point_first.x = x;
		mouse.point_first.y = y;
		break;
	case EVENT_LBUTTONUP:
		mouse.is_selection_started = false;
		mouse.is_selection_finished = true;
		mouse.point_second.x = x;
		mouse.point_second.y = y;
		break;
	case EVENT_MOUSEMOVE:
		if (mouse.is_selection_finished = false)
		{
			mouse.point_second.x = x;
			mouse.point_second.y = y;
		}
	}
	
}
int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);
  Mat img,src,dst;
  string imageName("C:/MyProjects/itseez-ss-2016-practice/test/test_data/imgproc/lena.png");
  if (argc > 1)
	imageName = argv[1];
  img = imread(imageName.c_str(), 1);
  if (img.empty())                      
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
  cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);
  cvSetMouseCallback("Original", OnMouse, &img);
  mouse.point_first.x = 10;
  mouse.point_first.y = 10;
  mouse.point_second.x = 400;
  mouse.point_second.y = 400;
  //IplImage tmp = img;
  while (1) 
  {
	  char c = cvWaitKey(33);
	  imshow("Original", img);
	  Rect roi(mouse.point_first, mouse.point_second);
	  rectangle(img, roi, 1);
	  ImageProcessorImpl obj;

	  //dst = obj.CvtColor(img, roi);
	  dst = obj.Filter(img, roi, 11);
	  imshow("Result", dst);
	  if (c == 27) 
	  { 
		  break;
	  }
  }

  //// If help option is given, print help message and exit.
  //if (parser.get<bool>("help")) {
  //  parser.printMessage();
  //  return 0;
  //}

  //// Do something cool.
  //cout << "This is empty template sample." << endl;
  waitKey(0);
  return 0;
}

