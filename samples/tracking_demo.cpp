#include <iostream>
#include "detection.hpp"
#include "tracking.hpp"

using namespace std;
using namespace cv;

const char* kOptions =
"{ v video        | <none> | video to process                         }"
"{ c camera       | <none> | camera to get video from                 }"
"{ h ? help usage |        | print help message                       }";



Rect_<int> roi;
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

	if (event == EVENT_LBUTTONDOWN)
	{
		static vector<Point2d> points;
		points.push_back(Point_<int>(x, y));
		if (points.size() > 1) {
			roi = Rect2i(points[0], points[1]);
			points.clear();
		}
	}

}

int main(int argc, const char** argv) {
	// Parse command line arguments.
	CommandLineParser parser(argc, argv, kOptions);

	// If help option is given, print help message and exit.
	if (parser.get<bool>("help")) {
		parser.printMessage();
		return 0;
	}

	std::string filePath;
	if (parser.has("v")) {
		Mat frame;
		VideoCapture video(parser.get<std::string>("v"));
		MedianFlowTracker tracker;
		video.read(frame);
		imshow("Result", frame);
		cvtColor(frame, frame, CV_BGR2GRAY);
		setMouseCallback("Result", CallBackFunc, NULL);

		while (true) {
			waitKey(1);
			if (!roi.empty()) break;
		}
		tracker.Init(frame, roi);
		while (video.read(frame)) {
			Mat tmp = frame;
			tracker.Track(frame);
			cvtColor(frame, frame, CV_BGR2GRAY);
			rectangle(tmp, tracker.getRect(), Scalar(250, 150, 10));
			imshow("Result", tmp);
			if (waitKey(1) == 27) break;

		}

		video.release();
	}

	else {
		cerr << "Error load model";
		return -1;
	}


	return 0;
}

	
		