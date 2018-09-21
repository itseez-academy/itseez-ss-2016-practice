#include "image_processing.hpp"
#include "opencv2/opencv.hpp"
#include <cstddef>
using namespace cv;
//?
Mat ImageProcessorImpl::CvtColor(const Mat &src, const Rect &roi)
{
	Mat src_copy = src;
	Mat src_copy_roi = src_copy(roi);
	Mat dst_gray_roi, dst_gray;
	/*Mat dst_roi;
	std::vector<Mat> channels;
	cvtColor(src_copy_roi, dst_gray_roi, COLOR_BGR2GRAY);
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);
	merge(channels, dst_roi);
	src_copy_roi = dst_roi;*/
	cvtColor(src_copy, dst_gray, COLOR_BGR2GRAY);
	return dst_gray;
}

cv::Mat ImageProcessorImpl::Filter(const Mat &src, const Rect &roi, const int size)
{	
	Mat src_copy(src);
	Mat src_copy_roi = src_copy(roi);
	medianBlur(src_copy_roi,src_copy,size);
	return src_copy;
	//return cv::Mat(); ?
}

Mat ImageProcessorImpl::DetectEdges(const Mat &src, const Rect &roi,
	const int filterSize, const int lowThreshold, const int ratio,
	const int kernelSize)
{
	Mat src_copy(src);
	Mat src_copy_roi = src_copy(roi);
	Mat src_gray_roi;
	cvtColor(src_copy_roi, src_gray_roi, COLOR_BGR2GRAY);
	Mat gray_blurred;
	Mat detected_edges;
	//blur(src_gray_roi, gray_blurred,);
	Canny(gray_blurred, detected_edges, lowThreshold, ratio);
	Mat dst(src);
	Mat dst_roi = dst(roi);
	Scalar::all(0);
	//dst_roi = src_roi;

	return Mat();
}

cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat & src, const cv::Rect & roi, const int divs)
{
	return cv::Mat();
}
