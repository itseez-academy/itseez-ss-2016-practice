#include "image_processing.hpp"
#include <cstddef>

using namespace std;
using namespace cv;

cv::Mat ImageProcessorImpl::Filter(const cv::Mat &src, const cv::Rect &roi, const int kSize)
{
	cv::Mat src_copy = src.clone();
	cv::Mat src_copy_roi = src_copy(roi);
	cv::medianBlur(src_copy_roi, src_copy_roi, kSize);
	return src_copy;
}

