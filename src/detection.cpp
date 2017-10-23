#include "detection.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {
    if (name == "cascade") {
        return std::make_shared<CascadeDetector>();
    }
}

bool CascadeDetector::Init(const std::string &model_file_path) {
    detector.load(model_file_path);
    return !detector.empty();
}

void CascadeDetector::Detect(const cv::Mat &frame, std::vector<cv::Rect> &objects,
                             std::vector<double> &scores) {
    if(detector.empty()){
        std::cerr << "detector is empty ";
        exit(-1);
    }
    std::vector<int>levels;
    detector.detectMultiScale(frame, objects);
}