#include "image_processing.hpp"

#include <cstddef>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

Mat MyImageProc::CvtColor(const Mat &src, const Rect &roi)
{
	Mat src_copy = src.clone();
	Mat src_copy_roi = src_copy(roi);
	Mat dst_gray_roi;
	cvtColor(src_copy_roi, dst_gray_roi, CV_8UC1, 0);
	vector<Mat>channels{ dst_gray_roi, dst_gray_roi, dst_gray_roi };
	Mat dst_rgb_roi;
	merge(channels, dst_rgb_roi);
	dst_rgb_roi.copyTo(src_copy_roi);

	return src_copy;
		
}

Mat MyImageProc::Filter(const Mat &src, const Rect &roi, const int kSize)
{
	Mat src_copy = src.clone();
	Mat src_copy_roi = src_copy(roi);
	Mat dst;
	medianBlur(src_copy_roi, dst, kSize);
	dst.copyTo(src_copy_roi);
	
	return src_copy;
}

Mat MyImageProc::DetectEdges(const Mat &src, const Rect &roi,
	const int filterSize, const int lowThreshold, const int ratio,
	const int kernelSize)
{
	Mat src_copy = src.clone();
	Mat src_roi = src_copy(roi);
	Mat src_gray_roi;
	cvtColor(src_roi, src_gray_roi, CV_8UC1, 0);
	Mat gray_blurred;
	blur(src_gray_roi, gray_blurred, Size(kernelSize, kernelSize));
	Mat detected_edges;
	Canny(gray_blurred, detected_edges, lowThreshold,255);

	Mat dst=src.clone();
	Mat dst_roi = dst(roi);
	dst_roi = Scalar::all(0);
	//dst_roi.setTo(Scalar::all(0));
	src_roi.copyTo(dst_roi, detected_edges);
	return dst;
}

Mat MyImageProc::Pixelize(const cv::Mat &src, const cv::Rect &roi,
	const int kDivs)
{
	Mat src_copy = src.clone();
	Mat src_roi_copy = src_copy(roi);
	int block_size_x = roi.width / kDivs, int block_size_y = roi.height / kDivs;

	for (int x = 0; x < roi.width; x += block_size_x)
	{
		for (int y = 0; y < roi.height; y += block_size_y)
		{
			Rect block(x, y, x + block_size_x, y + block_size_y);
			Mat blur_block = src_roi_copy(block);
			blur_block=mean(src_roi_copy(block));
		}
	}
	return src_copy;
}