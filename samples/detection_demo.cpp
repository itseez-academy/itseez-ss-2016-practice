#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"

#include "benchmark.hpp"
#include "detection.hpp"

using namespace std;
using namespace cv;

const char* kAbout = "This is a detection sample application.";

const char* kOptions =
    "{ v video        | <none>  | video to process                         }"
    "{ c camera       | <none>  | camera to get video from                 }"
    "{ d detector     | cascade | detector type cascade/hog                }"
    "{ gt             | <none>  | file with ground truth objects locations }"
    "{ m model        | <none>  |                                          }"
    "{ h ? help usage |         | print help message                       }";

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
  if (parser.has("video")) {
    string video_path = parser.get<string>("video");
    video.open(video_path);
    if (!video.isOpened()) {
      cout << "Failed to open video file '" << video_path << "'" << endl;
      return 0;
    }
  } else if (parser.has("camera")) {
    int camera_id = parser.get<int>("camera");
    video.open(camera_id);
    if (!video.isOpened()) {
      cout << "Failed to capture video stream from camera " << camera_id
           << endl;
      return 0;
    }
  }

  const string kWindowName = "video";
  const int kWaitKeyDelay = 20;
  const int kEscapeKey = 27;
  const Scalar kColorCyan = CV_RGB(0, 255, 255);
  const Scalar kColorGreen = CV_RGB(0, 255, 0);
  const int kLineThickness = 2;

  namedWindow(kWindowName);

  shared_ptr<Detector> detector =
      Detector::CreateDetector(parser.get<string>("detector"));
  string detector_model_file_path = parser.get<string>("model");
  if (!detector->Init(detector_model_file_path)) {
    std::cerr << "Failed to load detector from file '"
              << detector_model_file_path << "'";
    return 0;
  }

  GroundTruthReader gt_reader;
  if (parser.has("gt")) {
    gt_reader.Open(parser.get<string>("gt"));
  }
  DetectionQualityEvaluator benchmark;

  Mat frame;
  video >> frame;
  vector<Rect> gt_objects;
  gt_reader.Get(gt_objects);

  while (!frame.empty()) {
    vector<Rect> objects;
    vector<double> scores;
    detector->Detect(frame, objects, scores);
    benchmark.UpdateMetrics(objects, gt_objects);
    for (const auto& object : objects) {
      rectangle(frame, object, kColorCyan, kLineThickness);
    }
    for (const auto& object : gt_objects) {
      rectangle(frame, object, kColorGreen, kLineThickness);
    }
    imshow(kWindowName, frame);
    int key = waitKey(kWaitKeyDelay) & 0x00FF;
    if (key == kEscapeKey) {
      break;
    }
    video >> frame;
    gt_reader.Next(gt_objects);
  }

  if (gt_reader.IsOpen()) {
    cout << "Detection rate   :\t" << benchmark.GetDetectionRate() << endl;
    cout << "False alarms rate:\t" << benchmark.GetFalseAlarmRate() << endl;
  }

  return 0;
}
