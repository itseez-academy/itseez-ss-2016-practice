#include "detection.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {
  if (name == "cascade") {
    return std::make_shared<CascadeDetector>();
  } else {
    std::cerr << "Failed to create detector with name '" << name << "'"
              << std::endl;
    return nullptr;
  }
}

bool CascadeDetector::Init(const std::string& model_file_path) {
  return detector.load(model_file_path);
}

void CascadeDetector::Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
  std::vector<double>& scores) {
  if (detector.empty()) {
    std::cout << "Detector is not loaded" << std::endl;
    return;
  }

  std::vector<int> t;
  detector.detectMultiScale(frame, objects, t);

  scores.resize(t.size());
  int i = 0;
  for (auto it = t.begin(); it != t.end(); ++it) {
    scores[i++] = *it;
  }
}
