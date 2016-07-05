#pragma once
#include "image_processing.hpp"

using namespace std;
using namespace cv;

Mat ImageProcessorImpl::CvtColor(const cv::Mat &src, const cv::Rect &roi) {
	/*Mat res = src.clone();
	Mat res_roi = res(roi);
	Mat res_roi_gray;
	cvtColor(res_roi, res_roi_gray, CV_BGR2GRAY);
	cvtColor(res_roi_gray, res_roi_gray, CV_GRAY2BGR);
	res_roi_gray.copyTo(res_roi);
	return res;*/

	Mat src_copy = src.clone();
	Mat src_copy_roi = src_copy(roi);
	Mat dst_gray_roi;
	cvtColor(src_copy_roi, dst_gray_roi, CV_BGR2GRAY);
	vector<Mat> channels(src.channels(), dst_gray_roi);
	Mat dst_roi;
	merge(channels, dst_roi);
	dst_roi.copyTo(src_copy_roi);
	return src_copy;
}

Mat ImageProcessorImpl::Filter(const cv::Mat &src, const cv::Rect &roi,
	const int kSize) {
	return src;
}

Mat ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
	const int filterSize, const int lowThreshold, const int ratio,
	const int kernelSize) {
	return src;
}

Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi,
		const int kDivs) {
	return src;
}