#include <iostream>
#include <string>

#include "detection.hpp"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "image_processing.hpp"

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

  Mat src;
  if (!parser.has("image")) {
	  
	  return 0;
  }
  src = imread(parser.get<string>("image"));
  if (src.empty())
  {
	  cout << "image error" << endl;
	  return 0;
  }

  const string srcWinName = "Source image";
  namedWindow(srcWinName);
  imshow(srcWinName, src);
  const int kWaitTime = 0;
  waitKey(kWaitTime);

  CascadeDetector detectorFSample;
  if (!parser.has("model"))
  {
	  cout << "model error" << endl;
	  return 0;
  }
  detectorFSample.CascadeDetector::Init(parser.get<string>("model"));
  vector<Rect> objs;
  vector<double> scores;
  detectorFSample.CascadeDetector::Detect(src, objs, scores);
  //cout << objs.size() << endl;

  //for (int ind = 0; ind < objs.size(); ind++)
  //{
	 //// Mat rectSelect = objs[ind];
	 // Mat srcTemp;
	 // rectangle(srcTemp, objs[ind], Scalar(255, 0, 0));
	 // imshow(srcWinName, srcTemp);
	 // waitKey(kWaitTime);
  //}
  
  
	
  return 0;
}
