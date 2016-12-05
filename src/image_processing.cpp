#include "image_processing.hpp"

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

Mat ImageProcessorMax::CvtColor(const Mat &src, const Rect &roi)
{
	return tmp;
}

Mat ImageProcessorMax::Filter(const Mat &src, const Rect &roi,
	const int kSize) {
	return tmp;
}
Mat ImageProcessorMax::DetectEdges(const Mat &src, const Rect &roi,
	const int filterSize, const int lowThreshold, const int ratio,
	const int kernelSize) {
	Mat src_roi, src_gray_roi, gray_blurred, detected_edges, dst, dst_roi;
	
	tmp=src.clone();
	src_roi = tmp(roi);
	cvtColor(src_roi, src_gray_roi, CV_BGR2GRAY);
	cvtColor(src_gray_roi, src_gray_roi, CV_GRAY2BGR);
	blur(src_gray_roi, gray_blurred, Size(filterSize,filterSize));

 	Canny( gray_blurred, detected_edges, lowThreshold, lowThreshold*3, kernelSize);
	//gray_blurred.copyTo(tmp(roi));

	src.copyTo(dst);
	dst_roi=dst(roi);
	dst_roi = Scalar::all(0);
	src_roi.copyTo(dst_roi, detected_edges);	

	dst.copyTo(tmp(roi));
	return tmp;
}
Mat ImageProcessorMax::Pixelize(const Mat &src, const Rect &roi,
	const int kDivs) {
	return tmp;
}
