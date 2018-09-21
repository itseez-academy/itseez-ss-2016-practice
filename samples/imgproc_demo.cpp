#include <iostream>
#include <string>
#include <image_processing.hpp>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

//const char* kOptions =
//    "{ v video        |        | video to process         }"
//    "{ h ? help usage |        | print help message       }";

const char* kOptions =
"{ @image         | <none> | image to process            }"
"{ gray           |        | convert ROI to gray scale   }"
"{ median         |        | apply median filter for ROI }"
"{ edges          |        | detect edges in ROI         }"
"{ pix            |        | pixelize ROI                }"
"{ h ? help usage |        | print help message          }";

struct MouseCallbackState{
   bool is_selection_started;
   bool is_selection_finished;
   Point point_first;
   Point point_second;
};

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
  cout << "This is empty template sample." << endl;

  return 0;
}

