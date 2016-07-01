#pragma once

#include <memory>
#include <string>

#include "opencv2/core/core.hpp"

class ImageProcessor {
 public:
  virtual cv::Mat CvtColor(const cv::Mat &src, const cv::Rect &roi) = 0;
  virtual cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi,
                         const int size) = 0;
  virtual cv::Mat DetectEdges(const cv::Mat &src, const cv::Rect &roi,
                              const int filter_size, const int low_threshold,
                              const int ratio, const int kernel_size) = 0;
  virtual cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi,
                           const int divs) = 0;
};

class ImageProcessorImpl : public ImageProcessor {
 public:
  virtual cv::Mat CvtColor(const cv::Mat &src, const cv::Rect &roi);
  virtual cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi,
                         const int size);
  virtual cv::Mat DetectEdges(const cv::Mat &src, const cv::Rect &roi,
                              const int filter_size, const int low_threshold,
                              const int ratio, const int kernel_size);
  virtual cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi,
                           const int divs);
};
