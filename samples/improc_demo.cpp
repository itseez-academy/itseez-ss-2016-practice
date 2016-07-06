#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include"opencv2/imgproc.hpp"
#include"opencv2/highgui.hpp"
#include"image_processing.hpp"
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
Mat img = imread("C:\\Users/iss2016/Desktop/Александр Куликов/itseez-ss-2016-practice/test/test_data/imgproc/lena.png");
int mode = 0;
ImageProcessorImpl proc;
struct MouseCallbackState {
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
};


static void onMouse(int event, int x, int y, int, void* _state)
{
	
	MouseCallbackState* state = static_cast<MouseCallbackState*>(_state);
	if (state == nullptr)return;
	switch (event) {
	case  cv::EVENT_LBUTTONDOWN:

		state->is_selection_started = true;
		state->is_selection_finished = false;
		state->point_first = Point(x, y);
		break;

	case (cv::EVENT_LBUTTONUP) :

		state->is_selection_started = false;
		state->is_selection_finished = true;
		state->point_second = Point(x, y);
		break;


	case cv::EVENT_MOUSEMOVE:

		if (state->is_selection_started  && !state->is_selection_finished)state->point_second = Point(x, y);
		break;
	}

	if (state->is_selection_finished)
	{
		switch (mode)
		{
		case 0: break;
		case 1: img = proc.CvtColor(img, Rect(state->point_first, state->point_second)); break;
		case 2: img = proc.Filter(img, Rect(state->point_first, state->point_second), 15); break;
		case 3: img = proc.DetectEdges(img, Rect(state->point_first, state->point_second), 5, 1, 1, 5);
		case 4: img = proc.Pixelize(img, Rect(state->point_first, state->point_second), 1);
		}
		//rectangle(img, Rect(state->point_first, state->point_second), Scalar(255, 0, 0));
		//img = proc.CvtColor(img, Rect(state->point_first, state->point_second));
	imshow("win", img);
}
	
}

int main(int argc, const char** argv) {
	
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  
  mode = parser.has("gray") ? 1 : mode;
  mode = parser.has("median") ? 2 : mode;
  mode = parser.has("edges") ? 3 : mode;
  mode = parser.has("pix") ? 4 : mode;
  

  namedWindow("win");
  MouseCallbackState _state;
  _state.is_selection_finished = false;
  _state.is_selection_started = false;
  setMouseCallback("win", onMouse,&_state);
  // Do something cool.
  cout << "This is empty template sample." << endl;
  //  Mat img = imread(parser.get<string>(0));
 
  
  
  
  
  
  
  
  
  imshow("win", img);
  waitKey();
  return 0;
}
