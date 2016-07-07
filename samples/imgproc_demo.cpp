#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "image_processing.hpp"

using namespace std;

const char* kAbout = "Image processing sample.";

const char* kOptions =
	"{ @image         | <none> | image to process            }"
	"{ gray           |        | convert ROI to gray scale   }"
	"{ median         |        | apply median filter for ROI }"
	"{ edges          |        | detect edges in ROI         }"
	"{ pix            |        | pixelize ROI                }"
    "{ h ? help usage |        | print help message          }";
struct MouseCallbackState {
	bool isSelectionStarted;
	bool isSelectionFinished;
	cv::Point pointFirst;
	cv::Point pointSecond;

	MouseCallbackState(void) : isSelectionStarted(false), isSelectionFinished(false) {}
};
static void onMouse(int op, int x, int y, int, void* m) {
	MouseCallbackState* mt = (MouseCallbackState*)m;
	switch (op) {
	case cv::EVENT_LBUTTONDOWN:
		mt->isSelectionStarted = true;
		mt->isSelectionFinished = false;
		mt->pointFirst = cv::Point(x, y);
		break;
	case cv::EVENT_LBUTTONUP:
		mt->isSelectionStarted = false;
		mt->isSelectionFinished = true;
		mt->pointSecond = cv::Point(x, y);
		break;
	case cv::EVENT_MOUSEMOVE:
		if (mt->isSelectionStarted && mt->isSelectionFinished == false)
			mt->pointSecond = cv::Point(x, y);
		break;
	}
}
int main(int argc, const char** argv) {
  // Parse command line arguments.
  cv::CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  cv::Mat img = cv::imread(parser.get<string>(0));

  if (img.empty()) {
	  cout << "Error opening file " << parser.get<string>(0) << endl;
	  return 1;
  }

  const string basicWindowName = "Source";
  const int wk = 1;
  cv::namedWindow(basicWindowName);
  MouseCallbackState m;
  cv::setMouseCallback(basicWindowName, onMouse, &m);

	  cv::imshow(basicWindowName, img);
	  cv::Rect roi;
	  while (!m.isSelectionFinished) {
		  if (m.isSelectionStarted) {
			  roi = cv::Rect(m.pointFirst, m.pointSecond);
			  cv::Mat src(img.clone());
			  cv::rectangle(src, roi, cv::Scalar(CV_RGB(0, 255, 0)), 2);
			  cv::imshow(basicWindowName, src);
		  }
		  cv::waitKey(wk);
	  }

	  ImageProcessorImpl proc;
	  cv::Mat res;
	  if (parser.has("gray")) {
		  res = proc.CvtColor(img, roi);
	  }
	  else  if (parser.has("median")) {
		  const int fsize = 15;
		  res = proc.Filter(img, roi, fsize);
	  }
	  else  if (parser.has("edges")) {
		  const int fsize = 2;
		  const int lT = 75;
		  const int r = 3;
		  const int kS = 5;
		  res = proc.DetectEdges(img, roi, fsize, lT, r, kS);
	  }
	  else if (parser.has("pix")) {
		  const int d = 29;
		  res = proc.Pixelize(img, roi, d);
	  }

	  cv::namedWindow("Result");
	  cv::imshow("Result", res);
	  cv::waitKey();
  return 0;
}
