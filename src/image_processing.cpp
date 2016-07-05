#include "image_processing.hpp"
#include <memory>
#include <string>

#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

Mat ImageProcessorImpl::CvtColor(const Mat &src, const Rect &roi)
{

	Mat src_copy;
	src.copyTo(src_copy);

	Mat src_copy_roi = src_copy(roi);

	Mat dst_gray_roi;
	dst_gray_roi = CvtColor(src_copy_roi, roi);

	vector<Mat> channels;
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);

	Mat dst_roi;
	merge(channels, dst_gray_roi);

	dst_roi.copyTo(src_copy_roi);

	return src_copy;

}

Mat ImageProcessorImpl::Filter(const cv::Mat &src, const cv::Rect &roi, const int kSize)
{
	Mat src_copy;
	src.copyTo(src_copy);

	Mat src_copy_roi = src_copy(roi);

	medianBlur(src_copy_roi, src_copy_roi, kSize);

	return src_copy;
}

Mat ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi, const int filterSize, const int lowThreshold, const int ratio, const int kernelSize)
{
	Mat src_copy;
	src.copyTo(src_copy);

	Mat src_roi = src_copy(roi);

	Mat src_gray_roi = CvtColor(src_roi, roi);

	Mat gray_blurred;

	medianBlur(src_gray_roi, gray_blurred, filterSize);

	Mat detected_edges;

	Canny(gray_blurred, detected_edges, lowThreshold, lowThreshold*ratio, kernelSize);

	Mat dst;
	src.copyTo(dst);
	Mat dst_roi = dst(roi);

	dst_roi.zeros;

	src_roi.copyTo(dst, detected_edges);

	return dst;

}


Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi, const int kDivs)
{
	Mat src_copy;
	src.copyTo(src_copy);

	Mat src_copy_roi = src_copy(roi);

	int block_size_x = roi.width / kDivs;
	int block_size_y = roi.height / kDivs;


	for (int x = 0; x < roi.width; x += block_size_x)
	{
		for (int y = 0; y < roi.height; y += block_size_y)
		{
			Mat sub = src_copy_roi(Rect(x, y, block_size_x, block_size_y));
			
			blur(sub, sub, Size(block_size_x,block_size_y));
		}
	}

	return src_copy;

}
