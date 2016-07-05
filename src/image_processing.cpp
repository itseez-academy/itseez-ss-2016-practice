#include "image_processing.hpp"

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

Mat ImageProcessorMax::CvtColor(const Mat &src, const Rect &roi)
{
	//Mat tmp;
	tmp=src.clone();
	Mat sub = tmp(roi);
	cvtColor(sub, sub, CV_BGR2GRAY);
	cvtColor(sub, sub, CV_GRAY2RGB);
	//cvSetImageROI(image, cvRect(x, y, width, height));
	//roi.copyTo(src);
	sub.copyTo(tmp(roi));
	return tmp;
}

Mat ImageProcessorMax::Filter(const Mat &src, const Rect &roi,
	const int kSize) {
	return tmp;
}
Mat ImageProcessorMax::DetectEdges(const Mat &src, const Rect &roi,
	const int filterSize, const int lowThreshold, const int ratio,
	const int kernelSize) {
	return tmp;
}
Mat ImageProcessorMax::Pixelize(const Mat &src, const Rect &roi,
	const int kDivs) {
	return tmp;
}