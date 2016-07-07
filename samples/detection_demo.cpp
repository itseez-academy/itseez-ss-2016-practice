#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "detection.hpp"

using namespace std;
using namespace cv;

const char* kAbout = "Detection of images sample.";

const char* kOptions =
	"{ i image        | <none> | image to process                      }"
	"{ v video        | <none> | video to process                      }"
	"{ c camera       | <none> | camera to get video from              }"
	"{ d detector     | cascade |                                      }"
	"{ m model        |        |                                       }"
    "{ h ? help usage |        | print help message                    }";

bool detectFrame(Mat&, shared_ptr<Detector>&);
int detectOnImage(const CommandLineParser&);
int detectOnVideo(const CommandLineParser&);
int detectOnCamera(const CommandLineParser&);
int isCorrectExecution(void) {
	cout << "To complete, press any key." << endl;
	getchar();
	return 0;
}
int isIncorrectExecution(void) {
	cout << "To complete, press any key." << endl;
	getchar();
	return 1;
}

int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return isCorrectExecution();
  }

  if (parser.has("image")) return detectOnImage(parser);
  if (parser.has("video")) return detectOnVideo(parser);
  if (parser.has("camera")) return detectOnCamera(parser);

  return isCorrectExecution();
}

bool detectFrame(Mat& img, shared_ptr<Detector>& detector) {
	if (img.empty())
		return false;
	vector<double> sc;
	vector<Rect> ob;
	detector->Detect(img, ob, sc);
	for (Rect r : ob) {
		rectangle(img, r, Scalar(CV_RGB(0, 255, 0)));
	}

	const string basicWindow = "Source";
	namedWindow(basicWindow);
	imshow(basicWindow, img);
	return true;
}

int detectOnImage(const CommandLineParser& parser) {
	Mat img = imread(parser.get<string>("image"));
	if (img.empty()) {
		cout << "Error opening image " << parser.get<string>(0) << endl;
		return isIncorrectExecution();
	}
	shared_ptr<Detector> detector = Detector::CreateDetector(parser.get<string>("detector"));
	detectFrame(img, detector);
	waitKey(0);
	return isCorrectExecution();
}

int detectOnVideo(const CommandLineParser& parser) {
	VideoCapture video;
	video.open(parser.get<string>("video"));
	if (video.isOpened()) {
		while (true) {
			Mat frame;
			video >> frame;
			if (frame.empty()) break;
			shared_ptr<Detector> detector = Detector::CreateDetector(parser.get<string>("detector"));
			detectFrame(frame, detector);
			if (waitKey(25) != -1)
				break;
		}
		return isCorrectExecution();
	}
	cout << "Error opening video " << parser.get<string>(1) << endl;
	return isIncorrectExecution();
}

int detectOnCamera(const CommandLineParser& parser) {
	VideoCapture cam;
	cam.open(parser.get<string>("video"));
	if (cam.isOpened()) {
		while (true) {
			Mat frame;
			cam >> frame;
			if (frame.empty()) break;
			shared_ptr<Detector> detector = Detector::CreateDetector(parser.get<string>("detector"));
			detectFrame(frame, detector);
			if (waitKey(25) != -1)
				break;
		}
		return isCorrectExecution();
	}
	cout << "Error opening video " << parser.get<string>(2) << endl;
	return isIncorrectExecution();
}
