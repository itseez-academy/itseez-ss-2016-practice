#include "ImageProcessorImpl.hpp"

cv::Mat ImageProcessorImpl::CvtColor(const cv::Mat &src, const cv::Rect &roi) {

	cv::Mat src_tmp = src.clone();
	cv::Mat src_tmp_roi = src_tmp(roi);
	cv::Mat dst_roi_green = src_tmp_roi.clone();
	std::vector<cv::Mat> channels;
	
	cv::cvtColor(src_tmp_roi, dst_roi_green, cv::COLOR_BGR2GRAY);
	channels.push_back(dst_roi_green);
	channels.push_back(dst_roi_green);
	channels.push_back(dst_roi_green);
	cv::merge(channels, src_tmp_roi);

	return src_tmp;
}

cv::Mat ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
	const int filter_size, const int low_threshold,
	const int ratio, const int kernel_size) {


	cv::Mat src_tmp = src.clone();
	cv::Mat src_tmp_roi = src_tmp(roi);
	cv::Mat src_roi_green = src_tmp_roi.clone();
	cv::Mat gray_blurred;
	cv::Mat detected_edges;
	

	cv::cvtColor(src_tmp_roi, src_roi_green, cv::COLOR_BGR2GRAY);
	cv::blur(src_roi_green, gray_blurred, cv::Size(kernel_size, kernel_size));
	cv::Canny(gray_blurred, detected_edges, 0, 50);
	
	cv::Mat dst = src.clone();
	cv::Mat dst_roi = dst(roi);
	dst_roi = cv::Scalar::all(0);
	src_tmp_roi.copyTo(dst_roi, detected_edges);

	return dst;
}


cv::Mat ImageProcessorImpl::Filter(const cv::Mat &src, const cv::Rect &roi,
	const int size) {

	cv::Mat src_tmp = src.clone();
	cv::Mat src_tmp_roi = src_tmp(roi);
	cv::medianBlur(src_tmp_roi, src_tmp_roi, size);

	return src_tmp;
}

cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi,
	const int divs) {
 	cv::Mat src_tmp = src.clone();
	cv::Mat src_tmp_roi = src_tmp(roi);
	const int block_size_x = roi.width / divs, block_size_y = roi.height / divs;

	for (int x = 0; x < src_tmp_roi.cols - block_size_x ; x += block_size_x)
		for (int y = 0; y < src_tmp_roi.rows - block_size_y; y += block_size_y) {
			cv::Mat src_roi_block = src_tmp_roi(cv::Rect(x, y, block_size_x, block_size_y));
			cv::blur(src_roi_block, src_roi_block, cv::Size(block_size_x, block_size_y));
		}

	return src_tmp;
	
}