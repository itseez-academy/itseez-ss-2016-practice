#include <iostream>
#include "detection.hpp"

using namespace std;
using namespace cv;

const char* kOptions =
"{ i image        | <none> | image to process                         }"
"{ v video        | <none> | video to process                         }"
"{ c camera       | <none> | camera to get video from                 }"
"{ m model        | <none> | path to detector file                    }"
"{ h ? help usage |        | print help message                       }";

void DetectOnVideo(VideoCapture vid, CascadeDetector &detector) {
	std::vector<cv::Rect> objects;
	std::vector<double>  scores;
	while (true) {
		Mat frame;
		bool successRead = vid.read(frame);
		if (!successRead) break;
		detector.Detect(frame, objects, scores);
		for (auto obj : objects)
			rectangle(frame, obj, Scalar(250, 150, 10));
		imshow("Result", frame);
		if (waitKey(10) == 27) break;
	}

}



int main(int argc, const char** argv) {
	// Parse command line arguments.
	CommandLineParser parser(argc, argv, kOptions);

	// If help option is given, print help message and exit.
	if (parser.get<bool>("h")) {
		parser.printMessage();
		return 0;
	}

	CascadeDetector detector;
	std::vector<cv::Rect> objects;
	std::vector<double>  scores;
	if (parser.has("m")) detector.Init(parser.get<std::string>("m"));
	



	if (parser.has("i")) {
		Mat img = imread(parser.get<std::string>("i"));
		detector.Detect(img, objects, scores);
		if (scores.size() > 0) {
			namedWindow("Result");
			rectangle(img, objects[0], Scalar(250, 150, 10));
			imshow("Result", img);
		}
		else return 0;
	}
	else if (parser.has("v")) {
		VideoCapture video(parser.get<std::string>("v"));
		DetectOnVideo(video, detector);
		video.release();
	}
	else if (parser.has("c")) {

		VideoCapture cap(0);
		if (!cap.isOpened())
			return -1;
		DetectOnVideo(cap, detector);
		cap.release();
	}

	waitKey(0);

	return 0;
}

	
		