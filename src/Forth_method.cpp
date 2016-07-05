#include "image_processing.hpp"
#include <cstddef>

using namespace std;
using namespace cv;

cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi, const int kDivs)
{
	cv::Mat src_copy = src.clone();
	cv::Mat src_copy_roi = src_copy(roi);
	double block_size_x = roi.width / kDivs;
	double block_size_y = roi.height / kDivs;
	

	return src_copy;
}

