#include "image_processing.hpp"
#include <cstddef>

using namespace std;
using namespace cv;

cv::Mat ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi, const int filterSize, const int lowThreshold, const int ratio, const int kernelSize)
{
	cv::Mat src_copy = src.clone();
	cv::Mat src_roi = src_copy(roi);
	cv::Mat src_gray_roi;
	cv::Mat gray_blurred;
	cv::Mat detected_edges;
	cv::Mat dst;
	cv::cvtColor(src_roi, src_gray_roi, COLOR_BGR2GRAY);
	cv::blur(src_gray_roi, gray_blurred, Size(filterSize, filterSize), Point(1, 1), 4);
	cv::Canny(gray_blurred, detected_edges, lowThreshold, lowThreshold * ratio, kernelSize);
	src.copyTo(dst);
	cv::Mat dst_roi = dst(roi);
	dst_roi = Scalar::all(0);
	src_roi.copyTo(dst_roi, detected_edges);
	return dst;
}

