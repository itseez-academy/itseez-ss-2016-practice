#include <opencv2/imgproc/imgproc.hpp>
#include <image_processing.hpp>
using namespace std;
using namespace cv;
Mat ImageProcessorImpl::CvtColor(const cv::Mat &src, const cv::Rect &roi)
{
	Mat src_copy, src_copy_roi, roi, dst_gray_roi, dst_roi;
	src.copyTo(src_copy);
	src_copy_roi = src_copy(roi);

	cvtColor(src_copy_roi, dst_gray_roi, COLOR_BGR2GRAY);

	vector<Mat> channels;
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);
	merge(channels, dst_roi);
	cvtColor(dst_roi, dst_roi, COLOR_GRAY2BGR);
	dst_roi.copyTo(src_copy_roi);

	return src_copy;
}
Mat ImageProcessorImpl::Filter(const cv::Mat &src, const cv::Rect &roi,
	const int kSize)
{

}
Mat ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
	const int filterSize, const int lowThreshold, const int ratio,
	const int kernelSize)
{

}
Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi,
	const int kDivs)
{

}