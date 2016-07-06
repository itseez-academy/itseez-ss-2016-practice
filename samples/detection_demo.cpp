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

void showDetection(const Mat & image, std::shared_ptr<Detector> detector)
{
	//TODO: it probably would be better to pass detected rects and window name instead of detector

	std::vector<cv::Rect> objects;
	std::vector<double> scores;

	Mat newMat = image.clone();

	detector->Detect(image, objects, scores);

	for (int i = 0; i < objects.size(); i++)
	{
		cv::rectangle(newMat, objects.at(i), cv::Scalar(0, 0, 0));
	}

	const string windowName = "After detection";
	namedWindow(windowName, WINDOW_NORMAL);
	resizeWindow(windowName, newMat.cols, newMat.rows);
	imshow(windowName, newMat);
}

int main(int argc, const char** argv) 
{
	// Parse command line arguments.
	CommandLineParser parser(argc, argv, kOptions);
	parser.about(kAbout);

	// If help option is given, print help message and exit.
	if (parser.get<bool>("help")) 
	{
		parser.printMessage();
		return 0;
	}
  
	std::shared_ptr<Detector> detector = Detector::CreateDetector("cascade");
	bool noModel = true;

	if (detector == nullptr)
		cout << "\nCan't create detector\n";
	else if (parser.has("model"))
	{
		string modelPath = parser.get<string>("model");
		noModel = !(detector->Init(modelPath));
	}
	
	if (noModel)
	{
		parser.printMessage();
		return 0;
	}

	//Detector is supposed to be working

	if (parser.has("image"))
	{
	  string imagePath = parser.get<string>("image");

	  Mat image = imread(imagePath);
	  if (image.empty()) {
		  cout << "Failed to open image file '" + imagePath + "'."
			  << endl;
		  return 1;
	  }

	  const string windowName = "Your image";
	  namedWindow(windowName, WINDOW_NORMAL);
	  resizeWindow(windowName, image.cols, image.rows);
	  imshow(windowName, image);

	  showDetection(image, detector);

	  waitKey();
  }
  else if (parser.has("video"))
  {
	  VideoCapture cap(parser.get<string>("video"));
	  if (!cap.isOpened())
		  return -1;

	  const string camWindowName = "Your video";
	  namedWindow(camWindowName, WINDOW_NORMAL);
	  resizeWindow(camWindowName, 640, 480);

	  const string detWindowName = "After detection";
	  namedWindow(detWindowName, WINDOW_NORMAL);
	  resizeWindow(detWindowName, 640, 480);
		  
		while (cap.isOpened())
		{
			Mat frame;
			cap >> frame;

			imshow(camWindowName, frame);


			std::vector<cv::Rect> objects;
			std::vector<double> scores;
			detector->Detect(frame, objects, scores);

			Mat newMat = frame.clone();

			int i = 0;
			while (i < objects.size())
			{
				cv::rectangle(newMat, objects.at(i), cv::Scalar(0, 0, 0));
				i++;
			}

			imshow(detWindowName, newMat);

			if (cv::waitKey(30) >= 0) break;
		}
  }
  else if (parser.has("camera"))
  {
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	const string camWindowName = "Your camera";
	namedWindow(camWindowName, WINDOW_NORMAL);
	resizeWindow(camWindowName, 640, 480);

	const string detWindowName = "After detection";
	namedWindow(detWindowName, WINDOW_NORMAL);
	resizeWindow(detWindowName, 640, 480);

	while (cap.isOpened())
	{
		Mat frame;
		cap >> frame;

		std::vector<cv::Rect> objects;
		std::vector<double> scores;
		detector->Detect(frame, objects, scores);

		Mat newMat = frame.clone();

		int i = 0;
		while (i < objects.size())
		{
			cv::rectangle(newMat, objects.at(i), cv::Scalar(0, 0, 0));
			i++;
		}

		imshow(camWindowName, frame);
		imshow(detWindowName, newMat);

		if (cv::waitKey(30) >= 0) break;
	}
  }

  return 0;
}