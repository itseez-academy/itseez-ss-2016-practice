#pragma once

#include <memory>
#include <string>
#include <opencv/cv.hpp>

#include "opencv2/core/core.hpp"

class ImageProcessor {
 public:
   virtual cv::Mat CvtColor(const cv::Mat &src, const cv::Rect &roi) = 0;
   virtual cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi, 
     const int kSize) = 0;
   virtual cv::Mat DetectEdges(const cv::Mat &src, const cv::Rect &roi, 
     const int filterSize, const int lowThreshold, const int ratio, 
     const int kernelSize) = 0;
   virtual cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi, 
     const int kDivs) = 0;
};

class ImageProcessorImpl : public ImageProcessor {
public:
    cv::Mat CvtColor(const cv::Mat &src, const cv::Rect &roi) override;

    cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi, const int kSize) override;

    cv::Mat
    DetectEdges(const cv::Mat &src, const cv::Rect &roi, const int filterSize, const int lowThreshold, const int ratio,
                const int kernelSize) override;
    cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi, const int kDivs) override;
};