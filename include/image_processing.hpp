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

class ImageProcessorImpl : public ImageProcessor
{
public:
    cv::Mat CvtColor(const cv::Mat &src, const cv::Rect &roi) override {

        cv::Mat src_copy;
        src.copyTo(src_copy);
        cv::Mat src_copy_roi = src_copy(roi);
        cv::Mat dst_gray_roi;
        cv::cvtColor(src_copy_roi, dst_gray_roi, CV_BGR2GRAY);
        std::vector<cv::Mat> channels(3);

        std::fill(channels.begin(), channels.end(), dst_gray_roi);
        cv::Mat dst_roi;
        cv::merge(channels, dst_roi);
        dst_roi.copyTo(src_copy_roi);
        return src_copy;
    }

    cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi, const int kSize) override {

        return cv::Mat();
    }

    cv::Mat
    DetectEdges(const cv::Mat &src, const cv::Rect &roi, const int filterSize, const int lowThreshold, const int ratio,
                const int kernelSize) override {
        return cv::Mat();
    }

    cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi, const int kDivs) override {
        return cv::Mat();
    }
};