#include <iostream>
#include <string>

#include "detection.hpp"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

const char* kAbout = "Practice 3";

const char* kOptions =
	"{ i image        | <none> | image to process                         }"
	"{ v video        | <none> | video to process                         }"
	"{ c camera       | <none> | camera to get video from                 }"
	"{ m model        | <none> |                                          }"
	"{ h ? help usage |        | print help message                       }";

const string beforeDetection = "Before detection";
const string afterDetection = "After detection";

void createNamedWindows(int width, int height)
{
	string name = beforeDetection;
	namedWindow(name, WINDOW_NORMAL);
	resizeWindow(name, width, height);

	name = afterDetection;
	namedWindow(name, WINDOW_NORMAL);
	resizeWindow(name, width, height);
}

void showDetection(const Mat & image, std::shared_ptr<Detector> detector)
{
	std::vector<cv::Rect> objects;
	std::vector<double> scores;

	Mat newMat = image.clone();

	detector->Detect(image, objects, scores);

	for (int i = 0; i < objects.size(); i++)
	{
		cv::rectangle(newMat, objects.at(i), cv::Scalar(0, 0, 0));
	}

	imshow(afterDetection, newMat);
}

int main(int argc, const char** argv)
{
	CommandLineParser parser(argc, argv, kOptions);
	parser.about(kAbout);

	std::shared_ptr<Detector> detector = Detector::CreateDetector("cascade");

	if (parser.get<bool>("help"))
	{
		parser.printMessage();
		return 0;
	}
	else if (!parser.has("model"))
	{
		cout << "Model is required" << endl;
		return 0;
	}
	else if (detector == nullptr)
	{
		return 0;
	}
	else if (!detector->Init(parser.get<string>("model")))
	{
		cout << "Can't load model" << endl;
		return 0;
	}

	if (parser.has("image"))
	{
		string imagePath = parser.get<string>("image");

		Mat image = imread(imagePath);
		if (image.empty()) 
		{
			cout << "Failed to open image file '" + imagePath + "'." << endl;
			return 1;
		}

		createNamedWindows(image.cols, image.rows);

		imshow(beforeDetection, image);
		showDetection(image, detector);

		waitKey();
	}
	else if (parser.has("video") || parser.has("camera"))
	{
		VideoCapture cap;

		if (parser.has("video"))
			cap = VideoCapture(parser.get<string>("video"));
		else
			cap = VideoCapture(0);

		if (!cap.isOpened())
		{
			cout << "Failed to open video capture." << endl;
			return 1;
		}

		createNamedWindows(640, 480);
		  
		for (;;)
		{
			Mat frame;
			cap >> frame;

			if (frame.empty()) break;

			imshow(beforeDetection, frame);
			showDetection(frame, detector);

			if (cv::waitKey(30) >= 0) break;
		}
	}

	return 0;
}