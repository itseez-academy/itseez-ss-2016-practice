#include "tracking.hpp"

#include <algorithm>
#include <iostream>

#include "opencv2/imgproc.hpp"
#include "opencv2/video/tracking.hpp"

using std::string;
using std::shared_ptr;
using std::vector;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
  if (name == "median_flow") {
    return std::make_shared<MedianFlowTracker>();
  } else {
    std::cerr << "Failed to create tracker with name '" << name << "'"
              << std::endl;
  }
  return nullptr;
}

bool MedianFlowTracker::Init(const Mat &frame, const Rect &roi) {
  if (frame.channels() == 3) {
    cvtColor(frame, frame_, CV_BGR2GRAY);
  }
  Rect image_bounding_rect(Point(0, 0), frame.size());
  if ((image_bounding_rect & roi) == roi) {
    position_ = roi;
    return true;
  } else {
    std::cerr << "ROI " << roi << "is outside of image.";
  }
  return false;
}

float MedianFlowTracker::Median(const vector<float> &v) const {
  auto values = v;
  size_t middle = values.size() / 2;
  std::nth_element(values.begin(), values.begin() + middle, values.end());
  return values[middle];
}

bool MedianFlowTracker::FilterCorners(vector<Point2f> &corners,
                                      vector<Point2f> &corners_next_frame,
                                      vector<uchar> &status,
                                      vector<float> &errors) const {
  for (int i = status.size() - 1; i >= 0; i--) {
    if (!status[i]) {
      status.erase(status.begin() + i);
      corners.erase(corners.begin() + i);
      corners_next_frame.erase(corners_next_frame.begin() + i);
      errors.erase(errors.begin() + i);
    }
  }
  if (corners.empty()) {
    return false;
  }
  vector<float> errors_copy(errors.size());
  std::copy(errors.begin(), errors.end(), errors_copy.begin());
  float median_error = Median(errors_copy);

  for (int i = errors.size() - 1; i >= 0; i--) {
    if (errors[i] > median_error) {
      errors.erase(errors.begin() + i);
      corners.erase(corners.begin() + i);
      corners_next_frame.erase(corners_next_frame.begin() + i);
      status.erase(status.begin() + i);
    }
  }
  if (corners.empty()) {
    return false;
  }

  return true;
}

bool MedianFlowTracker::ComputeMedianShift(const vector<Point2f> &corners,
                                           const vector<Point2f> &nextCorners,
                                           Point2f &shift) const {
  vector<float> shifts_x, shifts_y;
  for (size_t i = 0; i < corners.size(); ++i) {
    shifts_x.emplace_back(nextCorners.at(i).x - corners.at(i).x);
    shifts_y.emplace_back(nextCorners.at(i).y - corners.at(i).y);
  }
  float dx = Median(shifts_x);
  float dy = Median(shifts_y);
  shift = Point2f(dx, dy);
  return true;
}

bool MedianFlowTracker::ComputePointDistances(const vector<Point2f> &corners,
                                              vector<float> &dist) const {
  dist.clear();
  for (int i = 0; i < corners.size(); i++) {
    for (int j = i + 1; j < corners.size(); j++) {
      dist.push_back(cv::norm(corners.at(i) - corners.at(j)));
    }
  }
  return true;
}

bool MedianFlowTracker::ComputeDistScales(const vector<float> &dist,
                                          const vector<float> &dist_next_frame,
                                          vector<float> &scales) const {
  scales.clear();
  if (dist.size() != dist_next_frame.size()) {
    return false;
  }
  for (size_t i = 0; i < dist.size(); ++i) {
    if (dist.at(i) != 0.0f) {
      scales.emplace_back(dist_next_frame.at(i) / dist.at(i));
    }
  }
  if (scales.empty()) {
    return false;
  }
  return true;
}

bool MedianFlowTracker::ComputeScaleFactor(
    const vector<Point2f> &corners, const vector<Point2f> &corners_next_frame,
    float &scale) const {
  if (corners.size() <= 1 || corners_next_frame.size() <= 1) {
    return false;
  }
  vector<float> dist, dist_next_frame, scales;
  ComputePointDistances(corners, dist);
  ComputePointDistances(corners_next_frame, dist_next_frame);
  ComputeDistScales(dist, dist_next_frame, scales);
  scale = Median(scales);
  return true;
}

bool MedianFlowTracker::RestoreBoundingBox(
    const vector<Point2f> &corners, const vector<Point2f> &corners_next_frame,
    Rect &new_position) const {
  Point2f shift;
  ComputeMedianShift(corners, corners_next_frame, shift);

  float scale;
  if (!ComputeScaleFactor(corners, corners_next_frame, scale)) {
    return false;
  }

  new_position = position_ + Point(shift);
  new_position.width *= scale;
  new_position.height *= scale;
  Rect image_bounding_box(Point(0, 0), frame_.size());
  new_position = image_bounding_box & new_position;
  if (new_position.area() == 0) {
    return false;
  }

  return true;
}

Rect MedianFlowTracker::Track(const Mat &frame) {
  CV_Assert(!frame.empty());
  Mat object = frame_(position_);
  vector<Point2f> corners;

  const int kMaxCorners = 100;
  const double kQualityLevel = 0.01;
  const double kMinDistance = 5.0;
  goodFeaturesToTrack(object, corners, kMaxCorners, kQualityLevel,
                      kMinDistance);
  if (corners.empty()) {
    std::cout << "Tracked object is lost." << std::endl;
    return Rect();
  }

  for (auto &corner : corners) {
    corner += Point2f(position_.tl());
  }

  vector<Point2f> corners_next_frame;
  vector<uchar> status;
  vector<float> errors;
  Mat next_frame = frame.clone();
  if (next_frame.channels() == 3) {
    cvtColor(next_frame, next_frame, CV_BGR2GRAY);
  }
  calcOpticalFlowPyrLK(frame_, next_frame, corners, corners_next_frame, status,
                       errors);

  if (!FilterCorners(corners, corners_next_frame, status, errors)) {
    std::cout << "There are not enough points for tracking." << std::endl;
    return Rect();
  }

  Rect new_position;
  if (!RestoreBoundingBox(corners, corners_next_frame, new_position)) {
    std::cout << "There are not enough points to restore bounding box."
              << std::endl;
    return Rect();
  }

  position_ = new_position;
  frame_ = next_frame;
  return position_;
}
