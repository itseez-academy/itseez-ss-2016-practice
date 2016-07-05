#include "..\include\image_processing.hpp"
#include "opencv2\opencv.hpp"
using namespace std;
using namespace cv;

cv::Mat ImageProcessorImpl::CvtColor(const cv::Mat & src, const cv::Rect & roi)
{

	Mat src_copy(src);
	
	Mat src_copy_roi(src_copy,roi);

	Mat dst_gray_roi;

	cvtColor(src_copy_roi, dst_gray_roi,CV_BGR2GRAY);

	vector<Mat> channels{ dst_gray_roi, dst_gray_roi, dst_gray_roi };
	Mat dst_roi;
	merge(channels, dst_roi);

	dst_roi.copyTo(src_copy_roi);

	return src_copy;
}

cv::Mat ImageProcessorImpl::Filter(const cv::Mat & src, const cv::Rect & roi, const int size)
{
	Mat src_copy(src);
	Mat src_copy_roi(src_copy, roi);
	medianBlur(src_copy_roi,src_copy_roi,size);
	return src_copy;

}

cv::Mat ImageProcessorImpl::DetectEdges(const cv::Mat & src, const cv::Rect & roi, const int filter_size, const int low_threshold, const int ratio, const int kernel_size)
{
	return cv::Mat();
}

cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat & src, const cv::Rect & roi, const int divs)
{
	return cv::Mat();
}
