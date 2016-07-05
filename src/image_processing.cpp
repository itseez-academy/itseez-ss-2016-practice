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
	Mat dst_gray_roi, gray_blurred;
	Mat detected_edges;		
	Mat src_copy(src,roi);
	cvtColor(src_copy, dst_gray_roi, CV_BGR2GRAY);
	blur(dst_gray_roi, gray_blurred, Size(filter_size, filter_size));
	Canny(gray_blurred, detected_edges, low_threshold, low_threshold * ratio, kernel_size);
	Mat dst;
	src.copyTo(dst);
	Mat dst_roi(dst, roi);
	
	dst_roi = Scalar::all(0);

	src_copy.copyTo(dst_roi, detected_edges);
	return dst;
}

cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat & src, const cv::Rect & roi, const int divs)
{
	return cv::Mat();
}
