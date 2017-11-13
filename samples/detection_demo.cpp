#include "detection.hpp"
#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using std::string;
using std::vector;
using cv::imread;
using cv::imshow;
using cv::namedWindow;

const char* kOptions =
"{ i image        | <none> | image to process                         }"
"{ v video        | <none> | video to process                         }"
"{ c camera       | <none> | camera to get video from                 }"
"{ m model        | <none> | path to detector file                    }"
"{ h ? help usage |        | print help message                       }";

int main(int argc, const char** argv) {
  // Parse command line arguments.
  cv::CommandLineParser parser(argc, argv, kOptions);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  CascadeDetector det;

  string det_path = parser.get<string>("model");
  det.Init(det_path);

  cv::Mat img;
  std::vector<cv::Rect> obj;
  std::vector<double> score;

  string windName = "Source";
  namedWindow(windName);

  if (parser.has("image")) {  //  IMAGE
    img = imread(parser.get<string>("image"), cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "Failed to open image file" << std::endl;
        return 0;
    }

    imshow(windName, img);
    cv::waitKey(10);

    try{
      det.Detect(img, obj, score);
    } catch (std::runtime_error& err) {
      std::cout << err.what();
      return 0;
    }

    for (auto it = obj.begin(); it != obj.end(); ++it) {
      cv::rectangle(img, *it, cv::Scalar(0, 0, 255), 2);
    }

    imshow(windName, img);
    cv::waitKey();
  } else {  //  VIDEO
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
    cap >> img;
    if (img.empty()) {
      cap >> img;
    }

    for (;;) {
      try {
        det.Detect(img, obj, score);
      } catch (std::runtime_error& err) {
        std::cout << err.what();
        return 0;
      }
      for (auto it = obj.begin(); it != obj.end(); ++it) {
        cv::rectangle(img, *it, cv::Scalar(0, 0, 255), 2);
      }

      imshow(windName, img);

      char c = cv::waitKey(20);
      if (c == 27 || img.empty()) break;

      cap >> img;
    }
  }

  return 0;
}
