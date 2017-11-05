#include "tracking.hpp"

#include <iostream>
#include <opencv/cv.hpp>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
  if (name == "median_flow") {
      return std::make_shared<MedianFlowTracker>();
  }
  std::cerr << "Failed to create tracker with name '" << name << "'"
            << std::endl;
  return nullptr;
}

cv::Rect MedianFlowTracker::Track(const cv::Mat &frame) {
    std::vector<cv::Point> prevPts;
    std::vector<cv::Point> nextPts;
    std::vector<uchar>     status;
    std::vector<float>     err;
    int    maxCorners   = 10;
    double qualityLevel = 0.01;
    double minDistance  = 20;
    cv::goodFeaturesToTrack(frame, prevPts, maxCorners,
                            qualityLevel, minDistance );
    cv::calcOpticalFlowPyrLK(frame_, frame, prevPts, nextPts, status, err );
    for(int i = 0 ; i < status.size(); i++){
        if(status[i] == 0){
            nextPts.erase(nextPts.begin() + i);
            prevPts.erase(prevPts.begin() + i);
        }
    }


}

bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi) {
    frame_    = frame;
    position_ = roi;
    return !frame_.empty();
}
