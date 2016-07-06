#include <iostream>
#include <string>

#include "detection.hpp"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
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
//Image
  Mat src;
  const string srcWinName = "Source image";
  if (parser.has("image"))
  {
    src = imread(parser.get<string>("image"));
    if (src.empty())
    {
      cout << "image error" << endl;
      return 0;
     }
  namedWindow(srcWinName);
  imshow(srcWinName, src);
  const int kWaitTime = 0;
  waitKey(kWaitTime);
  }



//Model
  if (!parser.has("model"))
  {
	  cout << "model error" << endl;
	  return 0;
  } 
  CascadeDetector detectorFSample;
  vector<Rect> objs;
  vector<double> scores;
    detectorFSample.CascadeDetector::Init(parser.get<string>("model"));

//Image
  if (parser.has("image"))
  {
    detectorFSample.CascadeDetector::Detect(src, objs, scores);
    Mat srcTemp;
    src.copyTo(srcTemp);
        for (int ind = 0; ind < objs.size(); ind++)
        {
         rectangle(srcTemp, objs[ind], Scalar(255, 0, 0));
        }
    imshow(srcWinName, srcTemp);
    waitKey(0);
    return 0;
   }

 //Video
   if(parser.has("video"))
   {
     VideoCapture cap(parser.get<string>("video"));
     if(!cap.isOpened())
     {
        return -1;
     }
     const string srcWinNameVideo = "Source video";
     namedWindow(srcWinNameVideo);
     const int kWaitTimeVideo = 30;
     waitKey(kWaitTimeVideo);

         while(cap.read(src))
         {
            detectorFSample.CascadeDetector::Detect(src, objs, scores);

            for (int ind = 0; ind < objs.size(); ind++)
            {
                rectangle(src, objs[ind], Scalar(255, 0, 0));
            }
            imshow(srcWinNameVideo, src);
            waitKey(kWaitTimeVideo);
        }
    return 0;
   }

  return 0;
}
