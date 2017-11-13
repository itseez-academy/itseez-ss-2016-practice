#include "detection.hpp"

#include <iostream>
#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {
  if (name == "cascade") {
    return std::make_shared<CascadeDetector>();
  } else {
    throw std::invalid_argument("Failed to create tracker");
  }
}

bool CascadeDetector::Init(const std::string& model_file_path) {
  return detector.load(model_file_path);
}

void CascadeDetector::Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
  std::vector<double>& scores) {
  if (detector.empty()) {
    throw std::runtime_error("Detector is not loaded");
  }

  std::vector<int> t;
  detector.detectMultiScale(frame, objects, t);

  scores.resize(t.size());
  int i = 0;
  for (auto it = t.begin(); it != t.end(); ++it) {
    scores[i++] = *it;
  }
}
