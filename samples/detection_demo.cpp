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

void showDetection(Mat image, const std::string& model_file_path)
{
	std::shared_ptr<Detector> detector = Detector::CreateDetector("cascade");

	std::vector<cv::Rect> objects;
	std::vector<double> scores;

	if (detector->Init(model_file_path))
	{
		detector->Detect(image, objects, scores);
	}

	Mat newMat = image.clone();
	
	for (int i = 0; i < objects.size(); i++)
	{
		cv::rectangle(newMat, objects[i], cv::Scalar(0, 0, 0));
	}

	const string windowName = "After detection";
	namedWindow(windowName, WINDOW_NORMAL);
	resizeWindow(windowName, image.cols, image.rows);
	imshow(windowName, newMat);
}

int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

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
	  waitKey();
  }

  //TODO: allow video
  //TODO: allow camera



  return 0;
}
