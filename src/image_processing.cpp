#pragma once
#include "image_processing.hpp"

using namespace std;
using namespace cv;

Mat CertainImageProcessor::CvtColor(const cv::Mat &src, const cv::Rect &roi) {
	return src;
}

Mat CertainImageProcessor::Filter(const cv::Mat &src, const cv::Rect &roi,
	const int kSize) {
	return src;
}

Mat CertainImageProcessor::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
	const int filterSize, const int lowThreshold, const int ratio,
	const int kernelSize) {
	return src;
}

Mat CertainImageProcessor::Pixelize(const cv::Mat &src, const cv::Rect &roi,
		const int kDivs) {
	return src;
}