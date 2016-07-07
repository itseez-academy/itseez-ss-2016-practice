#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "image_processing.hpp"
#include "detection.hpp"

#include <ctype.h>

using namespace std;
using namespace cv;

Mat image;
Mat source;

bool backprojMode = false;
bool selectObject = false;
int trackObject = 0;
bool showHist = true;
Point origin;
Rect selection;
bool gray = false;
bool filter = false;
bool edge = false;
bool pixel = false;





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


int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }
  //cout << parser.get<string>(0);
  //cout << parser.get<string>(1);
  string ImageString = parser.get<string>("image");
  Mat imgdetect;
  CascadeDetector detector1;
  detector1.Init(parser.get<string>("model"));
  std::vector<cv::Rect> objects;
  std::vector<double> scores;
  cout << "c";
  if (!ImageString.empty())
  {
	  cout << "a";
	  source = imread(ImageString);// CV_LOAD_IMAGE_GRAYSCALE);
	  source.copyTo(image);
	  if (image.empty()) {
		  cout << "Failed to open image file '" + parser.get<string>("image") + "'."
			  << endl;
		  return 0;
	  }
	  
	  detector1.Detect(image, objects, scores);
	  cout << scores.size() << endl;
	  for (int i = 0; i < scores.size(); i++)
		  cout << scores.at(i) << endl;

	  
	  image.copyTo(imgdetect);
	  //for (int i = 0; i < scores.size(); i++)	  cout << scores.at(i) << endl;
	  for (int i = 0; i < objects.size(); i++)
		  rectangle(imgdetect, selection, Scalar(115, 115, 115));
	  // Show source image.

	  namedWindow("original", CV_WINDOW_AUTOSIZE);
	  
	  // получаем информацию о видео-файле
	  
  }
  const string kSrcWindowName = "Source image";
  const int kWaitKeyDelay = 0;
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  resizeWindow(kSrcWindowName, 640, 480);
  string VideoString = parser.get<string>("video");
  string CameraString = parser.get<string>("camera");
  cout << VideoString;
  if (!VideoString.empty())
  {
	  Mat frame;
	  cv::VideoCapture vc(VideoString);
	  while (1) {
		  //cout << "ee";
		  // получаем следующий кадр
		  vc >> frame;

		  if (frame.empty()) {
			  break;
		  }

		  // здесь можно вставить
		  // процедуру обработки
		  std::vector<cv::Rect> objects;
		  std::vector<double> scores;
		  detector1.Detect(frame, objects, scores);
		  //cout << scores.size() << endl;



		  frame.copyTo(imgdetect);
		  //for (int i = 0; i < scores.size(); i++)		  cout << scores.at(i) << endl;
		  for (int i = 0; i < objects.size(); i++)
			  rectangle(imgdetect, objects.at(i), Scalar(115, 115, 115));

		  // показываем кадр
		  imshow(kSrcWindowName, imgdetect);
		  char c = cvWaitKey(33);
		  if (c == 27) { // если нажата ESC - выходим
			  break;
		  }
	  }
  }
  if (!CameraString.empty())
  {
	  cout << "b";
	  Mat frame;
	  cv::VideoCapture vc(0);
	  while (1) {
		  //cout << "ee";
		  // получаем следующий кадр
		  vc >> frame;

		  if (frame.empty()) {
			  break;
		  }

		  // здесь можно вставить
		  // процедуру обработки
		  std::vector<cv::Rect> objects;
		  std::vector<double> scores;
		  detector1.Detect(frame, objects, scores);
		  //cout << scores.size() << endl;



		  frame.copyTo(imgdetect);
		  //for (int i = 0; i < scores.size(); i++)		  cout << scores.at(i) << endl;
		  for (int i = 0; i < objects.size(); i++)
			  rectangle(imgdetect, objects.at(i), Scalar(115, 115, 115));

		  // показываем кадр
		  imshow(kSrcWindowName, imgdetect);
		  char c = cvWaitKey(33);
		  if (c == 27) { // если нажата ESC - выходим
			  break;
		  }
	  }
  }
  imshow(kSrcWindowName, imgdetect);
  waitKey(kWaitKeyDelay);
  
  // Do something cool.
  cout << "This is empty template sample." << endl;

  return 0;
}
