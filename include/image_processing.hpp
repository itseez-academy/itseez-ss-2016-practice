#pragma once

#include <memory>
#include <string>

#include "opencv2/core/core.hpp"

class ImageProcessor {
 public:
   virtual cv::Mat CvtColor(const cv::Mat &src, const cv::Rect &roi) = 0;
   virtual cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi, int kSize) = 0;
   virtual cv::Mat DetectEdges(const cv::Mat &src, const cv::Rect &roi, 
     int filterSize, int lowThreshold, int ratio, int kernelSize) = 0;
   virtual cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi) = 0;
};

class ImageProcessorImpl {
 public:
   virtual cv::Mat CvtColor(const cv::Mat &src, const cv::Rect &roi);
   virtual cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi, int kSize);
   virtual cv::Mat DetectEdges(const cv::Mat &src, const cv::Rect &roi,
     int filterSize, int lowThreshold, int ratio, int kernelSize);
   virtual cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi);
};