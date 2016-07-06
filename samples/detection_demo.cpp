#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "detection.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
"{ i image        | <none> | image to process                         }"
"{ v video        | <none> | video to process                         }"
"{ c camera       | <none> | camera to get video from                 }"
"{ m model        | <none> |                                          }"
"{ h ? help usage |        | print help message                       }";


int getMode(CommandLineParser& parser)
{
	if (parser.has("i"))
		return 0;
	if (parser.has("v"))
		return 1;
	if (parser.has("c"))
		return 3;
}

void ImageProc(shared_ptr<CascadeDetector>& detector, Mat& src)
{
	vector<Rect> objects;
	vector<double> scores;

	detector->Detect(src, objects, scores);

	for each (Rect itm in objects)
	{
		rectangle(src, itm, Scalar(0, 255, 255));
	}
}

int imageDetect(CommandLineParser& parser, shared_ptr<CascadeDetector>& detector)
{
	Mat src = imread(parser.get<string>("i"), CV_LOAD_IMAGE_COLOR);
	if (src.empty()) {
		cout << "Failed to open image file '" + parser.get<string>(0) + "'."
			<< endl;
		return 0;
	}

	ImageProc(detector, src);

	const string kSrcWindowName = "Detection image";
	const int kWaitKeyDelay = 1;
	namedWindow(kSrcWindowName);
	imshow(kSrcWindowName, src);
	waitKey(0);
}

int videoDetect(VideoCapture& cap, CommandLineParser& parser, shared_ptr<CascadeDetector>& detector)
{
	if (!cap.isOpened()) {
		cout << "Fail." << endl;
		return -1;
	}

	for (;;)
	{
		Mat img;
		cap >> img; if (img.empty()) break;

		ImageProc(detector, img);

		imshow("Detection video", img);
		waitKey(25);
	}
	waitKey(0);
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

  string pathToDetector = parser.get<string>("model");
  shared_ptr<CascadeDetector> detector = CascadeDetector::CreateDetector("cascade");
  bool flag = detector->Init(pathToDetector);

  if (flag == false) {
	  cout << "Failfail." << endl;
	  return -1;
  }

  int mode = getMode(parser);

  if (mode == 0)
	  imageDetect(parser, detector);

  if (mode == 1)
  {
	  string src = parser.get<string>("v");
	  VideoCapture cap;
	  cap.open(src);
	  videoDetect(cap, parser, detector);
  }

  if (mode == 3)
  {
	  VideoCapture cap(0);
	  videoDetect(cap, parser, detector);
  }
	  

  return 0;
}
