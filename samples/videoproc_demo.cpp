#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "image_processing.hpp"

using namespace std;
using namespace cv;

const int kEscapeKey = 27;

const char* kAbout = "This is image processing sample application.";

const char* kOptions =
    "{ camera         | <none> | camera identifier           }"
    "{ video          | <none> | video file                  }"
    "{ gray           |        | convert ROI to grayscale    }"
    "{ median         |        | apply median filter to ROI  }"
    "{ edges          |        | detect edges in ROI         }"
    "{ pix            |        | pixelize ROI                }"
    "{ h ? help usage |        | print help message          }";

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

void PrintVideoProperties(const VideoCapture& video) {
  Size frame_size = Size(video.get(CV_CAP_PROP_FRAME_WIDTH),
                         video.get(CV_CAP_PROP_FRAME_HEIGHT));
  size_t fps = video.get(CV_CAP_PROP_FPS);
  cout << "Frame size: " << frame_size << ", " << fps << " FPS" << endl;
}

int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.has("help") || !(parser.has("video") || parser.has("camera"))) {
    parser.printMessage();
    return 0;
  }

  // Read video.
  VideoCapture capture;
  if (parser.has("camera")) {
    capture.open(parser.get<int>("camera"));
  } else {
    capture.open(parser.get<string>("video"));
  }
  if (!capture.isOpened()) {
    cout << "Failed to get video." << endl;
    return 0;
  }
  PrintVideoProperties(capture);

  // Show source video and get ROI.
  const string kSrcWindowName = "Source video";
  namedWindow(kSrcWindowName);
  MouseCallbackState mouse_state;
  mouse_state.is_selection_started = false;
  mouse_state.is_selection_finished = false;
  setMouseCallback(kSrcWindowName, OnMouse, &mouse_state);

  Mat frame;
  capture >> frame;
  capture >> frame;  // First frame contains only black pixels.
  if (frame.empty()) {
    cout << "First frame is empty." << endl;
    return 0;
  }

  const int kWaitKeyDelay = 25;
  const Scalar kColorRed = CV_RGB(255, 0, 0);
  const int kLineThickness = 3;
  imshow(kSrcWindowName, frame);
  waitKey(kWaitKeyDelay);
  Rect roi;
  while (!mouse_state.is_selection_finished) {
    if (mouse_state.is_selection_started) {
      roi = Rect(mouse_state.point_first, mouse_state.point_second);
      Mat frame_to_show = frame.clone();
      rectangle(frame_to_show, roi, kColorRed, kLineThickness);
      imshow(kSrcWindowName, frame_to_show);
    }
    waitKey(kWaitKeyDelay);
  }

  const string kDstWindowName = "Destination video";
  namedWindow(kDstWindowName);
  char key = -1;
  do {
    Mat frame_copy;
    frame.copyTo(frame_copy);
    rectangle(frame_copy, roi, kColorRed, kLineThickness);
    imshow(kSrcWindowName, frame_copy);
    waitKey(kWaitKeyDelay);

    // Apply filter.
    ImageProcessorImpl proc;
    Mat dst_frame;
    try {
      if (parser.has("gray")) {
        dst_frame = proc.CvtColor(frame, roi);
      } else if (parser.has("median")) {
        const int kSize = 15;
        dst_frame = proc.Filter(frame, roi, kSize);
      } else if (parser.has("edges")) {
        const int kFilterSize = 3;
        const int kLowThreshold = 50;
        const int kRatio = 3;
        const int kKernelSize = 3;
        dst_frame = proc.DetectEdges(frame, roi, kFilterSize, kLowThreshold,
                                     kRatio, kKernelSize);
      } else if (parser.has("pix")) {
        const int kDivs = 10;
        dst_frame = proc.Pixelize(frame, roi, kDivs);
      } else {
        dst_frame = frame;
      }
    } catch (exception ex) {
      cout << ex.what() << endl;
      return 0;
    }

    capture >> frame;

    // Show destination image.
    imshow(kDstWindowName, dst_frame);
    key = waitKey(kWaitKeyDelay);

  } while (!frame.empty() && key != kEscapeKey);

  return 0;
}
