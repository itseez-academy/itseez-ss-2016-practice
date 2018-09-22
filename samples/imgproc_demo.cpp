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

struct MouseCallbackState
{
   bool is_selection_started;
   bool is_selection_finished;
   Point point_first;
   Point point_second;
};
//MouseCallbackState mouse;
void OnMouse(int event, int x, int y, int flags, void* param)
{	
	MouseCallbackState* mouse = (MouseCallbackState*)param;

	switch (event)
	{
	case EVENT_LBUTTONDOWN:


		mouse->is_selection_started = true;
		mouse->is_selection_finished = false;
		mouse->point_first.x = x;
		mouse->point_first.y = y;
		break;
	case EVENT_LBUTTONUP:
		mouse->is_selection_started = false;
		mouse->is_selection_finished = true;
		if (mouse->point_first.x != x && mouse->point_first.y != y)
		{
			mouse->point_second.x = x;
			mouse->point_second.y = y;
		}
		//rectangle(,Rect(mouse->point_first,mouse->point_second));
		break;
	    case EVENT_MOUSEMOVE:
		if (mouse->is_selection_finished == false)
		{
			mouse->point_second.x = x;
			mouse->point_second.y = y;
		}
		break;
	}
	
}
int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);
  Mat img,src,dst;
  string imageName("C:/MyProjects/itseez-ss-2016-practice/test/test_data/imgproc/lena.png"); //ralative path?
  
  if (argc > 1)
	imageName = argv[1];
  
  img = imread(imageName.c_str(), 1);
  
  if (img.empty())                      
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
  namedWindow("Original", CV_WINDOW_AUTOSIZE);
  MouseCallbackState p;
  setMouseCallback("Original", OnMouse, &p);
  //cvSetMouseCallback("Original", OnMouse, &img); // cv and not cv functions
  p.point_first.x = 10;
  p.point_first.y = 10;
  p.point_second.x = 400;
  p.point_second.y = 400;

  while (1) 
  {
	  char c = waitKey(33);//?
	  imshow("Original", img);
	  if (p.point_first.x != 0 && p.point_second.x != 0 && p.point_first.y != 0 && p.point_second.y != 0)
	  {
	      Rect roi(p.point_first, p.point_second);
		  ImageProcessorImpl obj;
			//dst = obj.DetectEdges(img, roi, 1, 1, 1, 1);
		  dst = obj.Pixelize(img, roi, 10);
		  imshow("Result", dst);
	  }
	  //dst = obj.CvtColor(img, roi);
	 // dst = obj.Filter(img, roi, 11); // size must be odd
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

