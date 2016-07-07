#include "image_processing.hpp"

cv::Mat MyImageProcessor::CvtColor(const cv::Mat &src, const cv::Rect &roi)
{
	cv::Mat src_copy = src.clone();
	cv::Mat dst_gray_roi;

	cv::Mat src_copy_roi = src_copy(roi);
	cv::cvtColor(src_copy_roi, dst_gray_roi, cv::COLOR_BGR2GRAY);
	cv::cvtColor(dst_gray_roi, dst_gray_roi, cv::COLOR_GRAY2BGR);
	
	dst_gray_roi.copyTo(src_copy_roi);

	return src_copy;
}

cv::Mat MyImageProcessor::Filter(const cv::Mat &src, const cv::Rect &roi, const int kSize)
{
	cv::Mat src_copy = src.clone();

	cv::Mat src_copy_roi = src_copy(roi);
	cv::medianBlur(src_copy_roi, src_copy_roi, kSize);

	return src_copy;
}

cv::Mat MyImageProcessor::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
	const int filterSize, const int lowThreshold, const int ratio,
	const int kernelSize)
{
	cv::Mat src_copy = src.clone();
	cv::Mat dst = src.clone();
	cv::Mat src_gray_roi;
	cv::Mat gray_blurred;
	cv::Mat detected_edges;
	cv::Size_<int> size(filterSize, filterSize);

	cv::Mat src_roi = src_copy(roi);

	cv::cvtColor(src_roi, src_gray_roi, cv::COLOR_BGR2GRAY);
	cv::blur(src_gray_roi, gray_blurred, size);
	cv::Canny(gray_blurred, detected_edges, lowThreshold, lowThreshold * ratio);

	cv::Mat dst_roi = dst(roi);
	cv::Scalar scal;
	dst_roi = scal.all(0);

	src_roi.copyTo(dst_roi, detected_edges);

	return dst;
}

cv::Mat MyImageProcessor::Pixelize(const cv::Mat &src, const cv::Rect &roi, const int kDivs)
{
	cv::Mat src_copy = src.clone();
	cv::Mat src_roi = src_copy(roi);

	int block_size_x = roi.width / kDivs;
	int block_size_y = roi.height / kDivs;

	cv::Size_<int> size(block_size_x, block_size_y);

	for (int i = 0; i < roi.width; i += block_size_x)
		for (int j = 0; j < roi.height; j += block_size_y)
		{
			cv::Rect block (i, j, block_size_x, block_size_x);
			cv::Mat src_block_roi = src_roi(block);
			cv::blur(src_block_roi, src_block_roi, size);
		}

	return src_copy;
}