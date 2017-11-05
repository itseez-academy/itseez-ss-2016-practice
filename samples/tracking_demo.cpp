#include "tracking.hpp"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <string>
#include <iostream>

using std::string;
using cv::Point;
using cv::Rect;
using cv::Mat;

const char* kOptions =
"{ v video        | <none> | video to process                         }"
"{ c camera       | <none> | camera to get video from                 }";

struct MouseCallbackState {
  bool is_selection_started;
  bool is_selection_finished;
  Point point_first;
  Point point_second;
};

void OnMouse(int event, int x, int y, int, void *param) {
  MouseCallbackState* m = (MouseCallbackState*)param;

  switch (event) {
  case cv::EVENT_LBUTTONDOWN:
    m->is_selection_started = true;
    m->is_selection_finished = false;
    m->point_first.x = x;
    m->point_first.y = y;
    break;

  case cv::EVENT_LBUTTONUP:
    m->is_selection_started = false;
    m->is_selection_finished = true;
    m->point_second.x = x;
    m->point_second.y = y;
    break;

  case cv::EVENT_MOUSEMOVE:
    if (m->is_selection_started == true && m->is_selection_finished == false) {
      m->point_second.x = x;
      m->point_second.y = y;
    }
    break;
  }
}

int main(int argc, const char** argv) {
  cv::CommandLineParser parser(argc, argv, kOptions);

  cv::Mat img;
  cv::VideoCapture cap;

  if (parser.has("video")) {
    string v_path = parser.get<string>("video");
    cap.open(v_path);

    if (!cap.isOpened()) {
      std::cout << "Failed to get video." << std::endl;
      return 0;
    }
  } else {
    if (parser.has("camera")) {
      int id = parser.get<int>("camera");
      cap.open(id);

      if (!cap.isOpened()) {
        std::cout << "Failed to capture video from camera" << std::endl;
        return 0;
      }
    }
  }

  const string SrcWinName = "Source video";
  cv::namedWindow(SrcWinName);
  MouseCallbackState mouse;
  mouse.is_selection_started = false;
  mouse.is_selection_finished = false;
  cv::setMouseCallback(SrcWinName, OnMouse, &mouse);

  cap >> img;
  if (img.empty()) {
    cap >> img;
  }

  imshow(SrcWinName, img);
 // cv::waitKey(10);

  Rect selected;
  while (!mouse.is_selection_finished) {
    if (mouse.is_selection_started) {
      Mat cSrc;
      selected = Rect(mouse.point_first, mouse.point_second);
      img.copyTo(cSrc);
      rectangle(cSrc, selected, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);

      imshow(SrcWinName, cSrc);
    }
    cv::waitKey(20);
  }

  MedianFlowTracker track;

  track.Init(img, selected);
  cap >> img;

  for (;;) {
    Mat copy;
    img.copyTo(copy);

    selected = track.Track(img);
    rectangle(copy, selected, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
    imshow(SrcWinName, copy);

    cap >> img;

    char c = cv::waitKey(20);
    if (c == 27 || img.empty()) break;
  }
  
  return 0;
}
