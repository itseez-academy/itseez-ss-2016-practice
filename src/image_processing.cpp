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

};
cv::Mat ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
    const int filter_size, const int low_threshold,
    const int ratio, const int kernel_size){

};
cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi,
    const int divs){

};
