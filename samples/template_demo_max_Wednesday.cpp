#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

//#include "workaround.hpp"

#include "detection.hpp"


using namespace std;
using namespace cv;

const char* kAbout = "Application for practice #1.";
Mat dest;
const char* kOptions =
"{ @image         |        | image to process         }"
"{ t              |  128   | threshold                }"
"{ h ? help usage |        | print help message       }";

void addF(unsigned char* const data, const int width,
	const int height, const int threshold) {

	for (int k = width; k < height*(width - 1); k++)
	{
		int j = k % width;
		int i = k / height;
		data[i, j] = (data[(i - 1)*height + j - 1] + 2 * data[(i - 1)*height + j] + data[i - 1, j] +
			data[(i)*height + j - 1] + 2 * data[k] + data[k] +
			data[(i + 1)*height + j - 1] + data[(i + 1)*height + j - 1]) / 9;
	}
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

	// Read image.
	Mat src = imread(parser.get<string>(0), CV_LOAD_IMAGE_COLOR);
	
	if (src.empty()) {
		cout << "Failed to open image file '" + parser.get<string>(0) + "'."
			<< endl;
		return 0;
	}

	// Show source image.
	const string kSrcWindowName = "Source image";
	const int kWaitKeyDelay = 1;
	namedWindow(kSrcWindowName, WINDOW_NORMAL);
	resizeWindow(kSrcWindowName, 640, 480);
	imshow(kSrcWindowName, src);
	waitKey(kWaitKeyDelay);
	Mat frame;
	
	// Threshold data.
	CascadeDetector CD;
	std::vector<Rect> faces ;
	std::vector<double> scores;
	//std::shared_ptr<Detector> CreateDetector("cascade");
	

	//const int threshold = parser.get<int>("t");
	try {
		//cvtColor(src, src, CV_RGB2GRAY);
		//cvtColor(src, src, CV_GRAY2RGB);
		//processor.CvtColor(src, roi);
		//processor.Filter(src, roi, 20); 
		//processor.DetectEdges(src, roi,	3,  10, 2,3 );
		CD.Init("../../test/test_data/detection/cascades/intel_logo_cascade.xml");
		CD.Detect(src, faces, scores);
		for (int i = 0; i < faces.size(); i++)
			cv::rectangle(src, faces[i], CV_RGB(255, 0, 0));
	}
	catch (const std::exception& ex) {
		cout << ex.what() << endl;
		return 0;
	}

	// Show destination image.
	const string kDstWindowName = "Destination image";
	namedWindow(kDstWindowName, WINDOW_NORMAL);
	resizeWindow(kDstWindowName, 640, 480);
	
	imshow(kDstWindowName,src);
	waitKey(0);

	return 0;
}
