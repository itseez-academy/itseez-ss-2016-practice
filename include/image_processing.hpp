#pragma once

#include <memory>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

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

class ImageProcessorImpl : public ImageProcessor {
public:
	virtual cv::Mat CvtColor(const cv::Mat &src, const cv::Rect &roi);
	  virtual cv::Mat Filter(const cv::Mat &src, const cv::Rect &roi,
		const int kSize);
	  virtual cv::Mat DetectEdges(const cv::Mat &src, const cv::Rect &roi,
		const int filterSize, const int lowThreshold, const int ratio,
		const int kernelSize);
      virtual cv::Mat Pixelize(const cv::Mat &src, const cv::Rect &roi,
		const int kDivs) ; 
};
