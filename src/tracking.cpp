#include "tracking.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
    if (name == "median_flow") {
        return std::make_shared<MedianFlowTracker>();
    }
  return nullptr;
}
bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi){}
cv::Rect MedianFlowTracker::Track(const cv::Mat &frame){}