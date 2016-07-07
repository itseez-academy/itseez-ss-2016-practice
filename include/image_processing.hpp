#pragma once

#include <memory>
#include <string>

#include "opencv2/core/core.hpp"

#include"opencv2\opencv.hpp"

using namespace cv;
class ImageProcessor {
 public:
   virtual Mat CvtColor(const Mat &src, const Rect &roi) = 0;
   virtual Mat Filter(const Mat &src, const Rect &roi, 
     const int kSize) = 0;
   virtual Mat DetectEdges(const Mat &src, const Rect &roi, 
     const int filterSize, const int lowThreshold, const int ratio, 
     const int kernelSize) = 0;
   virtual Mat Pixelize(const Mat &src, const Rect &roi, 
     const int kDivs) = 0;
};

class ImageProcessorImpl : public ImageProcessor {
public:
	virtual Mat CvtColor(const Mat &src, const Rect &roi);
	
	virtual Mat Filter(const Mat &src, const Rect &roi,
		const int size);
	
	virtual Mat DetectEdges(const Mat &src, const Rect &roi,
		const int filter_size, const int low_threshold,
		const int ratio, const int kernel_size);
	
	virtual Mat Pixelize(const Mat &src, const Rect &roi,
		const int divs);
};