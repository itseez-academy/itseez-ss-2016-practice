#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

//const char* kAbout =
//    "This is an empty application that can be treated as a template for your "
//    "own doing-something-cool applications.";
//
//const char* kOptions =
//    "{ v video        |        | video to process         }"
//    "{ h ? help usage |        | print help message       }";
//
//
//int main(int argc, const char** argv) {
//  // Parse command line arguments.
//  CommandLineParser parser(argc, argv, kOptions);
//  parser.about(kAbout);
//
//  // If help option is given, print help message and exit.
//  if (parser.get<bool>("help")) {
//    parser.printMessage();
//    return 0;
//  }
//
//  // Do something cool.
//  cout << "This is empty template sample." << endl;
//
//  return 0;
//}
//int main(int argc, char** argv)
//{
//	Mat img, gray, edges;
//	// relative path??
//	string imageName("C:/MyProjects/itseez-ss-2016-practice/test/test_data/imgproc/lena.png");
//	
//	if (argc > 1)
//	{
//		imageName = argv[1];
//	}
//	img = imread(imageName.c_str(), 1);
//	
//	if (img.empty())                      // Check for invalid input
//	{
//		cout << "Could not open or find the image" << std::endl;
//		return -1;
//	}
//	//namedWindow("Display window", WINDOW_AUTOSIZE);
//	imshow("original", img);
//	cvtColor(img, gray, COLOR_BGR2GRAY);
//	GaussianBlur(gray, gray, Size(7, 7), 1.5);
//	Canny(gray, edges, 0, 50);
//	//namedWindow("Display window", WINDOW_AUTOSIZE);
//	imshow("result", edges);
//	imwrite("result.png", edges);
//	waitKey(0);
//	return 0;
//}

