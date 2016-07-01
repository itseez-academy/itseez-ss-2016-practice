#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "image_processing.hpp"

using namespace std;
using namespace cv;

const char* kAbout = "This is image processing sample application.";

const char* kOptions =
    "{ @image         | <none> | image to process            }"
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
  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  }

  // Read image.
  Mat src = imread(parser.get<string>(0));
  if (src.empty()) {
    cout << "Failed to open image file '" + parser.get<string>(0) + "'."
         << endl;
    return 0;
  }

  // Show source image and get ROI.
  const string kSrcWindowName = "Source image";
  const int kWaitKeyDelay = 1;
  const Size kDefaultWindowSize = Size(640, 480);
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  resizeWindow(kSrcWindowName, kDefaultWindowSize.width,
               kDefaultWindowSize.height);
  MouseCallbackState mouse_state;
  mouse_state.is_selection_started = false;
  mouse_state.is_selection_finished = false;
  setMouseCallback(kSrcWindowName, OnMouse, &mouse_state);
  imshow(kSrcWindowName, src);
  waitKey(kWaitKeyDelay);

  Rect roi;
  while (!mouse_state.is_selection_finished) {
    if (mouse_state.is_selection_started) {
      roi = Rect(mouse_state.point_first, mouse_state.point_second);
      Mat src_copy = src.clone();
      const Scalar kRedColor = CV_RGB(255, 0, 0);
      const int kLineThickness = 3;
      rectangle(src_copy, roi, kRedColor, kLineThickness);
      imshow(kSrcWindowName, src_copy);
    }
    waitKey(kWaitKeyDelay);
  }

  // Apply filter.
  ImageProcessorImpl proc;
  Mat dst;
  try {
    if (parser.has("gray")) {
      dst = proc.CvtColor(src, roi);
    } else if (parser.has("median")) {
      const int kSize = 15;
      dst = proc.Filter(src, roi, kSize);
    } else if (parser.has("edges")) {
      const int kFilterSize = 3;
      const int kLowThreshold = 50;
      const int kRatio = 3;
      const int kKernelSize = 3;
      dst = proc.DetectEdges(src, roi, kFilterSize, kLowThreshold, kRatio,
                             kKernelSize);
    } else if (parser.has("pix")) {
      const int kDivs = 10;
      dst = proc.Pixelize(src, roi, kDivs);
    } else {
      dst = src;
    }
  } catch (exception ex) {
    cout << ex.what() << endl;
    return 0;
  }

  // Show destination image.
  const string kDstWindowName = "Destination image";
  namedWindow(kDstWindowName, WINDOW_NORMAL);
  resizeWindow(kDstWindowName, kDefaultWindowSize.width,
               kDefaultWindowSize.height);
  imshow(kDstWindowName, dst);
  waitKey();

  return 0;
}
