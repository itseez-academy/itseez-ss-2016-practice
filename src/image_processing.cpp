#pragma once

#include "image_processing.hpp"

cv::Mat ImageProcessorImpl::CvtColor(const cv::Mat &src, const cv::Rect &roi){
	cv::Mat src_copy;
	src.copyTo(src_copy);
	cv::Mat src_copy_roi = src_copy(roi);
	cv::Mat dst_gray_roi;
	cvtColor(src_copy_roi, dst_gray_roi, COLOR_BGR2GRAY);
	std::vector <Mat> channels;
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);
	cv::Mat dst_roi;
	merge(channels, dst_roi);
	dst_roi.copyTo(src_copy_roi);

	return src_copy;

};

cv::Mat ImageProcessorImpl::Filter(const cv::Mat &src, const cv::Rect &roi,
    const int size){

    cv::Mat src_copy;
    src.copyTo(src_copy);
    cv::Mat src_copy_roi = src_copy(roi);
    medianBlur(src_copy_roi, src_copy_roi, size);

    return src_copy;
};
cv::Mat ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
    const int filter_size, const int low_threshold,
    const int ratio, const int kernel_size){

    cv::Mat src_roi = src(roi);
    cv::Mat src_gray_roi = CvtColor(src_roi, Rect(0, 0, src_roi.cols, src_roi.rows));
    cv::Mat gray_blurred;
    blur(src_gray_roi, gray_blurred, Size(filter_size, filter_size));
    cv::Mat detected_edges;
    Canny(gray_blurred, detected_edges, low_threshold, low_threshold*ratio, kernel_size);
    cv::Mat dst;
    src.copyTo(dst);
    cv::Mat  dst_roi = dst(roi);
    dst_roi = Scalar::all(0);
    src_roi.copyTo(dst_roi, detected_edges);

    return dst;
};
cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi,
    const int divs){

};
