#include "image_processing.hpp"

#include <cstddef>

using namespace std;
using namespace cv;

Mat ImageProcessorImpl::CvtColor(const cv::Mat &src, const cv::Rect &roi)
{

	Mat src_cpy;
	src.copyTo(src_cpy);
	Mat src_cpy_roi = src_cpy(roi);
	Mat dst1c;
	cvtColor(src_cpy_roi, dst1c, COLOR_BGR2GRAY);

	vector<Mat> channels;
	channels.push_back(dst1c);
	channels.push_back(dst1c);
	channels.push_back(dst1c);

	Mat dst3c;
	merge(channels, dst3c);
	dst3c.copyTo(src_cpy_roi);
	return src_cpy;
}

Mat ImageProcessorImpl::Filter(const cv::Mat &src, const cv::Rect &roi,
							const int kSize)
{
	Mat src_cpy = src;
	Mat src_cpy_roi = src_cpy(roi);
	medianBlur(src_cpy_roi, src_cpy_roi, kSize);
	
	return src_cpy;
}

Mat ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
							const int filterSize, const int lowThreshold, 
							const int ratio, const int kernelSize)
{
	Mat src_roi = src(roi);
	Mat src_gray_roi;
	cvtColor(src_roi, src_gray_roi, COLOR_BGR2GRAY);

	Mat src_blurred;
	blur(src_gray_roi, src_blurred, Size(filterSize, filterSize));

	Mat detected_edges;
	Canny(src_blurred, detected_edges, lowThreshold, lowThreshold + 100);

	Mat dst;
	src.copyTo(dst);
	Mat dst_roi = dst(roi);
	dst_roi = Scalar::all(0);
	
	src_roi.copyTo(dst_roi, detected_edges);

	return dst;
}

Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi,
							const int kDivs)
{
	Mat src_cpy;
	src.copyTo(src_cpy);
	Mat src_cpy_roi = src_cpy(roi);

	int block_size_x = roi.width / kDivs;
	int block_size_y = roi.height / kDivs;
	
	for (int i = 0; i < kDivs; i++)
		for (int j = 0; j < kDivs; j++)
		{
			Mat src_roi_block = src_cpy_roi(Rect(i*block_size_x, j*block_size_y, block_size_x, block_size_y));
			blur(src_roi_block, src_roi_block, Size(block_size_x, block_size_y));
		}

	return src_cpy;

}
