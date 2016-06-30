#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "image_processing.hpp"

using namespace std;
using namespace cv;

const int kEscapeKey = 27;

const char* kAbout =
    "This is image processing sample application.";

const char* kOptions =
    "{ @video         |        | video to process            }"
    "{ cvt            |        | convert ROI to gray scale   }"
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
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  // Read video
  VideoCapture capture(parser.get<String>(0));  
  if (!capture.isOpened()) {
    cout << "Failed to open video file '" + parser.get<String>(0) + "'." << endl;
    return 0;
  }

  // Show source video and get ROI
  const string srcWindowName = "Source video";
  const int kWaitKeyDelay = 100;
  namedWindow(srcWindowName);
  MouseCallbackState mouse_state;
  mouse_state.is_selection_started = false;
  mouse_state.is_selection_finished = false;
  setMouseCallback(srcWindowName, OnMouse, &mouse_state); 

  Size frameSize = Size(capture.get(CV_CAP_PROP_FRAME_WIDTH),
    capture.get(CV_CAP_PROP_FRAME_HEIGHT));
  size_t fps = capture.get(CV_CAP_PROP_FPS);

  cout << "Video file: " << parser.get<String>(0) << endl;
  cout << "Delay: " << kWaitKeyDelay << " ms" << endl;
  cout << "Frame size: " << frameSize << ", " << fps << " FPS" << endl;

  Mat frame;
  capture >> frame;
  if (frame.empty())
  {
    cout << "First frame is empty." << endl;
    return 0;
  }

  imshow(srcWindowName, frame);
  while (!mouse_state.is_selection_finished) {
    waitKey(kWaitKeyDelay);
  }
  Rect roi = Rect(mouse_state.point_first, mouse_state.point_second);

  const string dstWindowName = "Destination video";
  namedWindow(dstWindowName);
  char key = -1;
  do {
    Mat frameCopy;
    frame.copyTo(frameCopy);
    rectangle(frameCopy, roi, Scalar(0, 0, 255), 2);
    imshow(srcWindowName, frameCopy);
    waitKey(kWaitKeyDelay);

    // Apply filter
    ImageProcessorImpl proc;
    Mat dstFrame;
    try {
      if (parser.get<bool>("cvt")) {
        dstFrame = proc.CvtColor(frame, roi);
      }
      else if (parser.get<bool>("median")) {
        const int kSize = 15;
        dstFrame = proc.Filter(frame, roi, kSize);
      }
      else if (parser.get<bool>("edges")) {
        const int filterSize = 3;
        int lowThreshold = 50;
        const int ratio = 3;
        const int kernelSize = 3;
        dstFrame = proc.DetectEdges(frame, roi, filterSize, lowThreshold, ratio, kernelSize);
      }
      else if (parser.get<bool>("pix")) {
        const int kDivs = 10;
        dstFrame = proc.Pixelize(frame, roi, kDivs);
      }
      else
      {
        dstFrame = frame;
      }
    }
    catch (exception ex)
    {
      cout << ex.what() << endl;
      return 0;
    }

    capture >> frame;

    // Show destination image    
    imshow(dstWindowName, dstFrame);
    key = waitKey(kWaitKeyDelay);

  } while (!frame.empty() && key != kEscapeKey);

  return 0;
}
