#include "image_processing.hpp"

cv::Mat ImageProcessorImpl::CvtColor(const cv::Mat &src, const cv::Rect &roi) {
    cv::Mat src_copy;
    src.copyTo(src_copy);
    cv::Mat src_copy_roi = src_copy(roi);
    cv::Mat dst_gray_roi;
    cv::cvtColor(src_copy_roi, dst_gray_roi, CV_BGR2GRAY);
    std::vector<cv::Mat> channels(3);
    std::fill(channels.begin(), channels.end(), dst_gray_roi );
    cv::Mat dst_roi;
    cv::merge(channels, dst_roi);
    dst_roi.copyTo(src_copy_roi);
    return src_copy;
}

cv::Mat ImageProcessorImpl::Filter(const cv::Mat &src, const cv::Rect &roi, const int kSize) {
    cv::Mat src_copy;
    src.copyTo(src_copy);
    cv::Mat src_copy_roi = src_copy(roi);
    cv::medianBlur(src_copy_roi, src_copy_roi, kSize);
    return src_copy;
}

