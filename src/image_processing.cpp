#include "image_processing.hpp"
#include "opencv2\imgproc.hpp"

using namespace cv;
using namespace std;


Mat ImageProcessorImpl::CvtColor(const Mat &src, const Rect &roi)
{
	Mat src_copy(src);
	Mat src_copy_roi(src_copy,roi);
	Mat dst_gray_roi;
	
	cvtColor(src_copy_roi,dst_gray_roi, CV_BGR2GRAY);

	vector<Mat> channels;
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);

	Mat dst_roi;

	merge(channels, dst_roi);

	dst_roi.copyTo(src_copy_roi);

	return src_copy;
}


Mat ImageProcessorImpl::Filter(const Mat &src, const Rect &roi,
	const int kSize)
{
	Mat src_copy(src);
	Mat src_copy_roi(src_copy, roi);

	medianBlur(src_copy_roi, src_copy_roi, kSize);

	return src_copy;
}


Mat ImageProcessorImpl::DetectEdges(const Mat &src, const Rect &roi,
	const int filterSize, const int lowThreshold, const int ratio,
	const int kernelSize) 
{
	Mat src_copy(src);
	Mat src_roi(src_copy, roi);
	Mat src_gray_roi;

	cvtColor(src_roi, src_gray_roi, CV_BGR2GRAY);

	Mat gray_blurred;
	blur(src_gray_roi, gray_blurred, Size(filterSize,filterSize));

	Mat dst(src);
	Mat dst_roi(dst, roi);
	Mat detected_edges;
	Canny(gray_blurred, detected_edges, lowThreshold, lowThreshold+ratio);
	
	dst_roi=Scalar::all(0);

	src_roi.copyTo(dst_roi, detected_edges);
	return dst;
}


Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi,
	const int kDivs)
{
	Mat src_copy(src);
	Mat src_copy_roi(src_copy, roi);

	int block_size_x = roi.width / kDivs, block_size_y = roi.height / kDivs;
	for (int i = roi.x;i<=roi.x+roi.width ; i + block_size_x)
		for (int j = roi.y; i <= roi.y + roi.height; i + block_size_y)
		{

		}

	return src_copy;
}