#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"

#include "tracking.hpp"

using namespace std;
using namespace cv;

const char* kAbout = "This is tracking sample application.";

const char* kOptions =
    "{ v video        | <none> | video to process                }"
    "{ c camera       | <none> | camera id to capture video from }"
    "{ h ? help usage |        | print help message              }";

struct MouseCallbackState {
  bool is_selection_started;
  bool is_selection_finished;
  Point point_first;
  Point point_second;
};

static void OnMouse(int event, int x, int y, int, void* s) {
  MouseCallbackState* state = reinterpret_cast<MouseCallbackState*>(s);
  CV_Assert(state != nullptr);
  switch (event) {
    case cv::EVENT_LBUTTONDOWN:
      state->is_selection_started = true;
      state->is_selection_finished = false;
      state->point_first = Point(x, y);
      break;
    case cv::EVENT_LBUTTONUP:
      state->is_selection_finished = true;
      state->is_selection_started = false;
      break;
    case cv::EVENT_MOUSEMOVE:
      if (state->is_selection_started && !state->is_selection_finished) {
        state->point_second = Point(x, y);
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

  // Load input video.
  VideoCapture video;
  bool is_live_stream = false;
  if (parser.has("video")) {
    string video_path = parser.get<string>("video");
    video.open(video_path);
    is_live_stream = false;
  }
  if (parser.has("camera")) {
    video.open(parser.get<int>("camera"));
    is_live_stream = true;
  }

  if (!video.isOpened()) {
    cout << "Failed to open video." << endl;
    return 0;
  }

  const string kWindowName = "video";
  const int kWaitKeyDelay = 20;
  const int kEscapeKey = 27;
  const Scalar kColorBlue = CV_RGB(0, 0, 255);
  const Scalar kColorGreen = CV_RGB(0, 255, 0);
  const int kLineThickness = 2;

  namedWindow(kWindowName);
  MouseCallbackState mouse_state;
  mouse_state.is_selection_started = false;
  mouse_state.is_selection_finished = false;
  setMouseCallback(kWindowName, OnMouse, &mouse_state);

  Mat frame;
  Rect roi;
  video >> frame;
  imshow(kWindowName, frame);
  while (!mouse_state.is_selection_finished) {
	  Mat frame_with_selection = frame.clone();
    roi = Rect(mouse_state.point_first, mouse_state.point_second);
    rectangle(frame_with_selection, roi, kColorGreen, kLineThickness);
	  imshow(kWindowName, frame_with_selection);
    waitKey(kWaitKeyDelay);
    if (is_live_stream) {
      video >> frame;
    }
  }

  MedianFlowTracker tracker;
  tracker.Init(frame, roi);
  video >> frame;

  while (!frame.empty()) {
    roi = tracker.Track(frame);
    rectangle(frame, roi, kColorBlue, kLineThickness);
    imshow(kWindowName, frame);
    int key = waitKey(kWaitKeyDelay) & 0x00FF;
    if (key == kEscapeKey) {
      break;
    }
    video >> frame;
  }

  return 0;
}
