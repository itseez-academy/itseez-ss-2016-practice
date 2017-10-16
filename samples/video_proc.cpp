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
"{ video          |        | image to process            }"
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
    if (m->is_selection_started == true && m->is_selection_finished == false) {
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

  VideoCapture cap;
  string path = parser.get<string>("video");
  cap.open(path);

  if (!cap.isOpened()) {
    cout << "Failed to get video." << endl;
    return 0;
  }

  // Show source video and get ROI.
  const string SrcWinName = "Source video";
  namedWindow(SrcWinName, WINDOW_NORMAL);
  MouseCallbackState mouse;
  mouse.is_selection_started = false;
  mouse.is_selection_finished = false;
  setMouseCallback(SrcWinName, OnMouse, &mouse);

  Mat img;
  cap >> img;

  imshow(SrcWinName, img);
  waitKey(10);

  Rect selected;
  while (!mouse.is_selection_finished) {
    if (mouse.is_selection_started) {
      Mat cSrc;
      selected = Rect(mouse.point_first, mouse.point_second);
      img.copyTo(cSrc);
      rectangle(cSrc, selected, Scalar(0, 0, 255), 1, LINE_AA);
      
      imshow(SrcWinName, cSrc);
    }
    waitKey(20);
  }

  string DstWinName;
  for (;;) {
    Mat copy;
    img.copyTo(copy);
    imshow(SrcWinName, copy);
    waitKey(20);

    ImageProcessorImpl processor;
    Mat res;

    if (parser.get<bool>("gray")) {
      DstWinName = "gray image";
      res = processor.CvtColor(img, selected);
    } else {
      if (parser.get<bool>("median")) {
        DstWinName = "madian image";
        res = processor.Filter(img, selected, 15);
      } else {
        if (parser.get<bool>("edges")) {
          DstWinName = "edges image";
          res = processor.DetectEdges(img, selected, 3, 50, 3, 3);
        } else {
          if (!parser.get<bool>("pix")) {
            DstWinName = "pix image";
            res = processor.Pixelize(img, selected, 10);
          }
        }
      }
    }

    cap >> img;
    namedWindow(DstWinName);
    imshow(DstWinName, res);


    char c = waitKey(20);
    if (c == 27 || img.empty()) break;
  }
  
  return 0;
}
