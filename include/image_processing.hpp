#pragma once

#include <memory>
#include <string>

#include "opencv2/core/core.hpp"
#include"opencv2/imgproc.hpp"
#include"opencv2/highgui.hpp"
#include<vector>
class ImageProcessor {
 public:
   virtual cv::Mat CvtColor(const cv::Mat &src, const cv::Rect &roi) = 0;
   virtual cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi, 
     const int kSize) = 0;
   virtual cv::Mat DetectEdges(const cv::Mat &src, const cv::Rect &roi, 
     const int filterSize, const int lowThreshold, const int ratio, 
     const int kernelSize) = 0;
   virtual cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi, 
     const int kDivs) = 0;
};

class ImageProcessorImpl : public ImageProcessor{
public:
	virtual cv::Mat CvtColor(const cv::Mat &src, const cv::Rect &roi) {
		cv::Mat  src_copy = src.clone();
		cv::Mat src_copy_roi(src_copy(roi));
		cv::Mat dst_gray_roi;
		cv::cvtColor(src_copy_roi, dst_gray_roi, cv::COLOR_BGR2GRAY);
		std::vector<cv::Mat> channels;
		channels.push_back(dst_gray_roi);
		channels.push_back(dst_gray_roi);
		channels.push_back(dst_gray_roi);
		cv::Mat dst_roi;
		cv::merge(channels, dst_roi);
		dst_roi.copyTo(src_copy_roi);
		return src_copy;
	}
	virtual cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi,
		const int kSize) {
		cv::Mat  src_copy = src.clone();
		cv::Mat src_copy_roi(src_copy(roi));
		cv::medianBlur(src_copy_roi, src_copy_roi, kSize);
		return src_copy;
	}
	virtual cv::Mat DetectEdges(const cv::Mat &src, const cv::Rect &roi,
		const int filterSize, const int lowThreshold, const int ratio,
		const int kernelSize){
		cv::Mat src_roi(src(roi));
		cv::Mat src_gray_roi;
		cv::cvtColor(src_roi, src_gray_roi, cv::COLOR_BGR2GRAY);
		cv::Mat gray_blurred;
		cv::blur(src_gray_roi, gray_blurred, cv::Size(filterSize,filterSize));
		cv::Mat detected_edges;
		cv::Canny(gray_blurred, detected_edges, lowThreshold, ratio);
		cv::Mat dst = src.clone();
		cv::Mat dst_roi(dst(roi));
		dst_roi=cv::Scalar::all(0);
		
		cv::cvtColor(detected_edges, src_roi, cv::COLOR_GRAY2BGR);
		src_roi.copyTo(dst_roi);
		return dst;
	}
	virtual cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi,
		const int kDivs){

		cv::Mat src_copy = src.clone();
		cv::Mat src_ñopy_roi(src_copy(roi));
		int block_size_x = roi.width / kDivs, block_size_y = roi.height / kDivs;
		for (int x = 0; x < src_ñopy_roi.cols; x += block_size_x)
			for (int y = 0; y < src_ñopy_roi.rows; y += block_size_y)
			{
				cv::Mat tmp(src_ñopy_roi(cv::Rect(x, y, block_size_x, block_size_y)));
				blur(tmp, tmp, cv::Size(block_size_x, block_size_y));
			}

		return src_copy;
	}
};