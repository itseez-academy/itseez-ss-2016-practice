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
        cv::merge(dst_gray_roi, dst_gray_roi, dst_gray_roi, dst_roi);
        dst_roi.copyTo(src_copy_roi);
        return src_copy;
    }

    cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi, const int kSize) override {
        cv::Mat src_copy;
        src.copyTo(src_copy);
        cv::Mat src_copy_roi = src_copy(roi);
        cv::medianBlur(src_copy_roi, src_copy_roi, kSize);
        return src_copy;
    }

    cv::Mat
    DetectEdges(const cv::Mat &src, const cv::Rect &roi, const int filterSize, const int lowThreshold, const int ratio,
                const int kernelSize) override {
        cv::Mat src_copy;
        src.copyTo(src_copy);
        cv::Mat src_roi = src_copy(roi);
        cv::Mat src_gray_roi;
        cv::cvtColor(src_roi, src_gray_roi, CV_BGR2GRAY);
        cv::Mat detected_edges;
        cv::blur(src_gray_roi, src_gray_roi, cv::Size(filterSize, filterSize));
        cv::Canny(src_gray_roi, detected_edges, lowThreshold, lowThreshold + ratio, kernelSize);
        cv::Mat dst;
        src.copyTo(dst);
        cv::Mat dst_roi = dst(roi);
        dst_roi = cv::Scalar::all(0);
        src_roi.copyTo(dst_roi, detected_edges);
        return dst;
    }

    cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi, const int kDivs) override {
        cv::Mat src_copy;
        src.copyTo(src_copy);
        cv::Mat src_copy_roi = src_copy(roi);
        int block_size_x = roi.width / kDivs;
        int block_size_y = roi.height / kDivs;
        for (int i = 0; i < src_copy_roi.rows; i += block_size_y)
            for (int j = 0; j < src_copy_roi.cols; j += block_size_x) {
                cv::Mat src_roi_block = src_copy_roi(cv::Rect(j, i,
                                                          j + block_size_x <= src_copy_roi.cols ?
                                                          block_size_x : src_copy_roi.cols - j,
                                                          i + block_size_y <= src_copy_roi.rows ?
                                                          block_size_y : src_copy_roi.rows - i));

                cv::blur(src_roi_block, src_roi_block, cv::Size(block_size_x, block_size_y));
            }
        return src_copy;
    }
};
