#include "image_processing.hpp"
#include <cstddef>

using namespace std; 
using namespace cv;

cv::Mat ImageProcessorImpl::CvtColor(const cv::Mat & src, const cv::Rect & roi)
{
	cv::Mat src_copy = src.clone();
	cv::Mat src_copy_roi = src_copy(roi);
	cv::Mat dst_gray_roi;
	cv::Mat dst_roi;
	cv::cvtColor(src_copy_roi, dst_gray_roi, COLOR_BGR2GRAY);
	vector<Mat> chanels(3);
	chanels[0] = dst_gray_roi;
	chanels[1] = dst_gray_roi;
	chanels[2] = dst_gray_roi;
	merge(chanels, dst_roi);
    dst_roi.copyTo(src_copy_roi);
	return src_copy;
}

