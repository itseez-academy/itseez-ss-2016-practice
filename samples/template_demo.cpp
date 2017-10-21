#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "image_processing.hpp"

using namespace std;
using namespace cv;

const char* kAbout = "solution for lab2";

const char* kOptions =
                   "{ @image         |        | image to process            }"
                   "{ gray           |        | convert ROI to gray scale   }"
                   "{ median         |        | apply median filter for ROI }"
                   "{ edges          |        | detect edges in ROI         }"
                   "{ pix            |        | pixelize ROI                }"
                   "{ h ? help usage |        | print help message          }";

struct MouseCallbackState {
  bool is_selection_started;
  bool is_selection_finished;
  Point point_first;
  Point point_second;
};

void OnMouse(int event, int x, int y, int, void *param) {
  MouseCallbackState* m = (MouseCallbackState*)param;

  switch (event) {
    case EVENT_LBUTTONDOWN:
      m->is_selection_started = true;
      m->is_selection_finished = false;
      m->point_first.x = x;
      m->point_first.y = y;
      break;

    case EVENT_LBUTTONUP:
      m->is_selection_started = false;
      m->is_selection_finished = true;
      m->point_second.x = x;
      m->point_second.y = y;
      break;

    case EVENT_MOUSEMOVE:
      if (m->is_selection_started == true && m->is_selection_finished == false){
        m->point_second.x = x;
        m->point_second.y = y;
      }
      break;
  }
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

  // Read image.
  Mat src = imread(parser.get<string>(0), IMREAD_COLOR);
  if (src.empty()) {
    cout << "Failed to open image file '" + parser.get<string>(0) + "'."
      << endl;
    return 0;
  }

  // Show source image.
  string kSrcWindowName = "Source image";
  int kWaitKeyDelay = 1;
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  MouseCallbackState mouse;
  mouse.is_selection_started = false;
  mouse.is_selection_finished = false;

  setMouseCallback(kSrcWindowName, OnMouse, &mouse);
  imshow(kSrcWindowName, src);

  while (!mouse.is_selection_finished) {
    waitKey(100);
  }

  Mat cSrc;
  Rect selected = Rect(mouse.point_first, mouse.point_second);
  src.copyTo(cSrc);
  rectangle(cSrc, selected, Scalar(0, 0, 255), 1, LINE_AA);

  imshow(kSrcWindowName, cSrc);
  waitKey(10);

  ImageProcessorImpl processor;

  if (parser.get<bool>("gray")) {
    kSrcWindowName = "gray image";
    namedWindow(kSrcWindowName, WINDOW_NORMAL);
    imshow(kSrcWindowName, processor.CvtColor(src, selected));
  } else
    if (parser.get<bool>("median")) {
    kSrcWindowName = "madian image";
    kWaitKeyDelay = 1;
    namedWindow(kSrcWindowName, WINDOW_NORMAL);
    imshow(kSrcWindowName, processor.Filter(src, selected, 15));
  } else
   if (parser.get<bool>("edges")) {
    kSrcWindowName = "edges image";
    namedWindow(kSrcWindowName, WINDOW_NORMAL);
    imshow(kSrcWindowName, processor.DetectEdges(src, selected, 3, 50, 3, 3));
  } else
  if (parser.get<bool>("pix")) {
    kSrcWindowName = "pix image";
    namedWindow(kSrcWindowName, WINDOW_NORMAL);
    imshow(kSrcWindowName, processor.Pixelize(src, selected, 10));
  }

  waitKey();
  return 0;
}
