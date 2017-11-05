#include <iostream>
#include <string>
#include <detection.hpp>

#include "opencv2/core.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
        "{ i image        | <none> | image to process                         }"
        "{ v video        | <none> | video to process                         }"
        "{ c camera       | <none> | camera to get video from                 }"
        "{ m model        | <none> | path to detector file                    }"
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
  // Do something cool.
  CascadeDetector detector;
  if(parser.has("m")){
      if(!detector.Init(parser.get<string>("m"))){
          cerr << "Error load model" << endl;
          exit(-1);
      }
  }
  else{
      cerr << "not found model" << endl;
      exit(-1);
  }

  VideoCapture cap(0);
  if(!cap.isOpened()){
      cout << "Error opening video stream or file" << endl;
      return -1;
  }

  vector<Rect>   objects;
  vector<double> scores;
  Mat frame;
  namedWindow("detection");
  while(true){

      cap >> frame;
      detector.Detect(frame, objects, scores);
      for(const auto& obj : objects){
          rectangle(frame, obj, Scalar(0, 255, 0));
      }
      imshow("detection", frame);
      char c=(char)waitKey(25);
      if(c==27)
          break;
  }
  cap.release();
  return 0;
}
