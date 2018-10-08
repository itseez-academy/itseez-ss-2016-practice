#include <memory>
#include <string>
#include "tracking.hpp"

using namespace std;
using namespace cv;

struct MouseCallbackState
{
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
};

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
int main(int argc, const char** argv)
{	
	//init
	VideoCapture capture;
	Mat frame;
	Rect roi;
	MedianFlowTracker track;
	char c;
	MouseCallbackState p;
	std::string vidName = "C:\\MyProjects\\itseez-ss-2016-practice\\test\\test_data\\video\\pedestrians.mpg";
	bool flag = capture.open(vidName);
	namedWindow("Track");
	setMouseCallback("Track", OnMouse, &p);

	if (flag)
	{
		flag = capture.read(frame);
		
			if (frame.empty())
			{
				cout<< " --(!) No captured frame -- Break!";
				return -1;
			}
			
			imshow("Track", frame);
			// select obj
			while (1)
			{
				c = waitKey(1000);
				roi = Rect(p.point_first, p.point_second);
				if ((p.point_first.x < p.point_second.x) && (p.point_first.y < p.point_second.y) && !roi.empty()) break;
			}
			rectangle(frame, roi, Scalar(255, 0, 0));//draw rect
			imshow("Track", frame);
			
			if (!track.Init(frame, roi))
			{
				cout << "Init error";
				return -1;
			};
			while (capture.read(frame))
			{
				track.Track(frame);
				rectangle(frame, roi, Scalar(255, 0, 0));//draw rect
				imshow("Track", frame);
			}
		capture.release();
	}
	else
		cout << "--(!)Error opening video capture" << endl;
	
	return 0;
}