#include <string>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "ImageProcessorImpl.hpp"

using namespace std;
using namespace cv;

const char* kOptions =
"{ @image         |        | image to process            }"
"{ gray           |        | convert ROI to gray scale   }"
"{ median         |        | apply median filter for ROI }"
"{ edges          |        | detect edges in ROI         }"
"{ pix            |        | pixelize ROI                }"
"{ h ? help usage |        | print help message          }";




void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		static vector<Point2d> points;
		points.push_back(Point_<int>(x, y));
		if (points.size() > 1) {
			*reinterpret_cast<Rect_<int> *>(userdata) = Rect2i(points[0], points[1]);
			points.clear();
		}
	}
}

int main(int argc, const char** argv) {
	CommandLineParser parser(argc, argv, kOptions);
	if (parser.get<bool>("help")) {
		parser.printMessage();
		return 0;
	}
	Rect_<int> roi;
	Mat src = imread(parser.get<std::string>("v"));
	if (src.empty()) {
		cout << "Failed to open image file '" + parser.get<std::string>("v") + "'."
			<< endl;
		return 0;
	}

	imshow("Source image", src);
	setMouseCallback("Source image", CallBackFunc, &roi);

	while (true) {
		waitKey(1);
		if (!roi.empty()) break;
	}
	const string newWindowName = "filter";
	ImageProcessorImpl newFiltr;
	Mat img;
	if (parser.has("gray")) img = newFiltr.CvtColor(src, roi);
	if (parser.has("median")) img = newFiltr.Filter(src, roi, 7);
	if (parser.has("edges")) img = newFiltr.DetectEdges(src, roi, 2, 75, 3, 5);
	if (parser.has("pix")) img = newFiltr.Pixelize(src, roi, 15);	
	
	namedWindow(newWindowName, WINDOW_NORMAL);
	resizeWindow(newWindowName, 640, 480);
	imshow(newWindowName, img);
	
	waitKey(0);
	return 0;
}
