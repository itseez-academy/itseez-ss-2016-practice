#include <iostream>
#include "detection.hpp"

using std::string;
using std::shared_ptr;
using std::invalid_argument;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {
    if (name == "cascade") {
      return std::make_shared<CascadeDetector>();
    }
    else
        throw invalid_argument("invalid name expected cascade");



}


bool CascadeDetector::Init(const std::string &model_file_path) {
      detector.load(model_file_path);
      return !detector.empty();
}

void CascadeDetector::Detect(const cv::Mat &frame, std::vector<cv::Rect> &objects, std::vector<double> &scores) {
  if (!detector.empty()) {
    std::vector<int> sc;
    detector.detectMultiScale(frame, objects, sc);
    scores.resize(sc.size());
    std::copy(sc.begin(), sc.end(), scores.begin());
  }
  else {
    std::cerr << "Error";
    exit(-1);
  }
}
