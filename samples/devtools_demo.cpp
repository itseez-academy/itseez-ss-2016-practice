#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "ImageProcessorImpl.hpp"

#include "workaround.hpp"

using namespace std;
using namespace cv;

const char* kAbout = "Application for practice #1.";

const char* kOptions =
    "{ @image         |        | image to process            }"
	"{ gray           |        | convert ROI to gray scale   }"
	"{ median         |        | apply median filter for ROI }"
	"{ edges          |        | detect edges in ROI         }"
	"{ pix            |        | pixelize ROI                }"
	"{ h ? help usage |        | print help message          }";



Rect_<int> roi;
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	
	if (event == EVENT_LBUTTONDOWN)
	{
		static vector<Point2d> points;
		points.push_back(Point_<int>(x,y));
		if (points.size() > 1) {
			roi =  Rect2i(points[0], points[1]);
			points.clear();
		}
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
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  resizeWindow(kSrcWindowName, 640, 480);


  setMouseCallback(kSrcWindowName, CallBackFunc, NULL);
  imshow(kSrcWindowName, src);
  
  
  while (true) {
	  waitKey(1);
	  if (!roi.empty()) break;
  }
  const string newWindowName = "filter";
  ImageProcessorImpl newFiltr;
  //Mat img = newFiltr.CvtColor(src, roi);
  //Mat img = newFiltr.Filter(src, roi, 7);
  Mat img = newFiltr.DetectEdges(src, roi, 2, 75, 3 , 5);
  //Mat img = newFiltr.Pixelize(src, roi, 15);
  namedWindow(newWindowName, WINDOW_NORMAL);
  resizeWindow(newWindowName, 640, 480);
  imshow(newWindowName, img);

  
  waitKey(0);
  return 0;
}
