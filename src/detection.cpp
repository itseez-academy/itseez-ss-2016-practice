#include <iostream>
#include "detection.hpp"
#include <stdexcept>
using std::string;
using std::shared_ptr;
using std::vector;
using std::copy;
using cv::Mat;
using cv::Rect;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {
    if (name == "cascade") {
        return std::make_shared<CascadeDetector>();
    }
    throw std::invalid_argument("name != cascasde");

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
    vector<int> sc;
    detector.detectMultiScale(frame, objects, sc);
    scores.resize(sc.size());
    copy(sc.cbegin(), sc.cend(), scores.begin());
}