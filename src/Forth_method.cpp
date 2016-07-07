#include "image_processing.hpp"
#include <cstddef>

using namespace std;
using namespace cv;

cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi, const int kDivs)
{
	cv::Mat src_copy = src.clone();
	cv::Mat src_copy_roi = src_copy(roi);
	int block_size_x = roi.width / kDivs;
	int block_size_y = roi.height / kDivs;
	for (int i = 0; i < roi.width; i += block_size_x)
		for (int j = 0; j < roi.height; j += block_size_y)
		{
			Rect roi2(i, j, block_size_x, block_size_y);
			cv::Mat src_roi_block = src_copy_roi(roi2);
			medianBlur(src_roi_block, src_roi_block, 9);
		}

	return src_copy;
}

