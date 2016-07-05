#pragma once

#include <memory>
#include <string>

#include "opencv2/opencv.hpp"
#include <stdio.h>
using namespace std;
using namespace cv;
class ImageProcessor {
 public:
   virtual cv::Mat CvtColor(const cv::Mat &src, const cv::Rect &roi) = 0;
   virtual cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi, const int kSize) = 0;
   virtual cv::Mat DetectEdges(const cv::Mat &src, const cv::Rect &roi, const int filterSize, const int lowThreshold, const int ratio, const int kernelSize) = 0;
   /*virtual cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi, const int kDivs) = 0;*/
};

class ImageProcessorImpl : public ImageProcessor {
public:
	Mat CvtColor(const cv::Mat &src, const cv::Rect &roi)
	{
		Mat src_copy(src);
		Mat dst_gray_roi;
		Mat dst_roi;
		Mat src_copy_roi (src_copy,roi);
		cvtColor(src_copy_roi, dst_gray_roi, CV_BGR2GRAY);
		vector<Mat> channels {dst_gray_roi, dst_gray_roi, dst_gray_roi};
		merge(channels, dst_roi);
		dst_roi.copyTo(src_copy_roi);
		return src_copy;

	}
	Mat Filter(const cv::Mat &src, const cv::Rect &roi,const int size)
	{
		Mat src_copy(src);
		Mat src_copy_roi(src_copy, roi);
		medianBlur(src_copy_roi, src_copy_roi,size);
		return src_copy;
	}
	Mat DetectEdges(const Mat &src, const Rect &roi, const int filter_size, const int low_threshold, const int ratio, const int kernel_size)
	{
		Mat src_copy(src);
		Mat src_roi(src_copy, roi);
		Mat src_gray_roi;
		Mat gray_blurred;
		Mat dst;
		Mat detected_edges;
		cvtColor(src_roi, src_gray_roi, CV_BGR2GRAY);
		blur(src_gray_roi, gray_blurred, Size(filter_size, filter_size), Point(-1, -1), BORDER_DEFAULT);
		Canny(gray_blurred, detected_edges, low_threshold, low_threshold * ratio, kernel_size);
		src.copyTo(dst);
		Mat dst_roi(dst, roi);
		dst_roi=Scalar::all(0);
		src_roi.copyTo(dst_roi, detected_edges);
		return dst;
	}
	/*virtual cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi,
		const int divs);*/
};