#pragma once

#include <memory>
#include <string>

#include "opencv2/core/core.hpp"

class Tracker {
 public:
  static std::shared_ptr<Tracker> CreateTracker(const std::string &name);
  virtual bool Init(const cv::Mat &frame, const cv::Rect &roi) = 0;
  virtual cv::Rect Track(const cv::Mat &frame) = 0;
};

class MedianFlowTracker : public Tracker {
 public:
  virtual bool Init(const cv::Mat &frame, const cv::Rect &roi);
  virtual cv::Rect Track(const cv::Mat &frame);

 protected:
  cv::Rect position_;
  cv::Mat frame_;
  cv::Size initial_size_;
  float scale_;

  float Median(const std::vector<float> &v) const;

  bool FilterCorners(std::vector<cv::Point2f> &corners,
                     std::vector<cv::Point2f> &corners_next_frame,
                     std::vector<uchar> &status,
                     std::vector<float> &errors) const;

  bool ComputeMedianShift(const std::vector<cv::Point2f> &corners,
                          const std::vector<cv::Point2f> &corners_next_frame,
                          cv::Point2f &shift) const;

  bool ComputePointDistances(const std::vector<cv::Point2f> &corners,
                             std::vector<float> &dist) const;

  bool ComputeDistScales(const std::vector<float> &dist,
                         const std::vector<float> &dist_next_frame,
                         std::vector<float> &scales) const;

  bool ComputeScaleFactor(const std::vector<cv::Point2f> &corners,
                          const std::vector<cv::Point2f> &corners_next_frame,
                          float &scale) const;

  bool RestoreBoundingBox(const std::vector<cv::Point2f> &corners,
                          const std::vector<cv::Point2f> &corners_next_frame,
                          cv::Rect &new_position, float& scale) const;
};
