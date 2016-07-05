#include "image_processing.hpp"

#include <opencv2\imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat ImageProcessorImpl::CvtColor(const cv::Mat &src, const cv::Rect &roi)
{
	Mat newMat = src.clone();

	Mat roiMat = newMat(roi);
	Mat grayRoi;

	cvtColor(roiMat, grayRoi, CV_BGR2GRAY);

	std::vector<Mat> tmp;
	tmp.push_back(grayRoi);
	tmp.push_back(grayRoi);
	tmp.push_back(grayRoi);

	Mat dstRoi;
	merge(tmp, dstRoi);
	dstRoi.copyTo(roiMat);

	return newMat;
}

Mat ImageProcessorImpl::Filter(const cv::Mat &src, const cv::Rect &roi, const int size)
{
	if (size % 2 == 0)
		throw "wrong kernel size";

	Mat newMat = src.clone();

	Mat roiMat = newMat(roi);
	Mat dataMat = roiMat.clone();

	medianBlur(roiMat, dataMat, size);
	dataMat.copyTo(roiMat);

	return newMat;
}

/*
	This implementation uses kernel_size for Sobel and filter_size for blur
*/
Mat ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
	const int filter_size, const int low_threshold,
	const int ratio, const int kernel_size)
{
	Mat grayRoi, grayBlurred, edges;

	cvtColor(src(roi), grayRoi, CV_RGB2GRAY, 1);
	blur(grayRoi, grayBlurred, Size(filter_size, filter_size));
	Canny(grayBlurred, edges, low_threshold, ratio, kernel_size);

	Mat newMat = src.clone();
	newMat(roi).setTo(Scalar::all(0));
	src(roi).copyTo(newMat(roi), edges);

	return newMat;
}

Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi, const int divs)
{
	Mat newMat = src.clone();

	int pixelWidth = roi.width / divs;
	int pixelHeight = roi.height / divs;

	Size * pixelSize = new Size(pixelWidth, pixelHeight);
	Rect * pixelRect = new Rect(roi.x, roi.y, pixelWidth, pixelHeight);

	for (int i = 0; i < divs; i++)
		for (int j = 0; j < divs; j++)
		{
			pixelRect->x = roi.x + i * pixelWidth;
			pixelRect->y = roi.y + j * pixelHeight;
			

			Mat pixel = newMat(*pixelRect), blurredPixel;
			blur(pixel, blurredPixel, *pixelSize);

			blurredPixel.copyTo(pixel);
		}

	delete pixelSize;
	delete pixelRect;

	return newMat;
}