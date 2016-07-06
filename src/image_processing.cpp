#include "image_processing.hpp"

#include <opencv2\imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Rect getContainedRoi(const Mat & src, const Rect & roi)
{
	Rect newRoi = Rect();

	newRoi.x = max(0, roi.x);
	newRoi.y = max(0, roi.y);

	newRoi.width = roi.x < 0 ? roi.width + roi.x : roi.width;
	newRoi.height = roi.y < 0 ? roi.height + roi.y : roi.height;

	newRoi.width = min(newRoi.width, src.cols - newRoi.x);
	newRoi.height = min(newRoi.height, src.rows - newRoi.y);

	return newRoi;
}

Mat ImageProcessorImpl::CvtColor(const cv::Mat &src, const cv::Rect &roi)
{
	Rect containedRoi = getContainedRoi(src, roi);
	Mat newMat = src.clone();

	Mat roiMat = newMat(containedRoi);
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
	Rect containedRoi = getContainedRoi(src, roi);

	Mat newMat = src.clone();

	Mat roiMat = newMat(containedRoi);
	Mat dataMat = roiMat.clone();

	try
	{
		medianBlur(roiMat, dataMat, size);
	}
	catch (...)
	{
		throw;
	}

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
	Rect containedRoi = getContainedRoi(src, roi);

	Mat grayRoi, grayBlurred, edges;

	cvtColor(src(containedRoi), grayRoi, CV_RGB2GRAY, 1);
	blur(grayRoi, grayBlurred, Size(filter_size, filter_size));
	Canny(grayBlurred, edges, low_threshold, ratio, kernel_size);

	Mat newMat = src.clone();
	newMat(containedRoi).setTo(Scalar::all(0));
	src(containedRoi).copyTo(newMat(containedRoi), edges);

	return newMat;
}

Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi, const int divs)
{
	Rect containedRoi = getContainedRoi(src, roi);
	Mat newMat = src.clone();

	int pixelWidth = containedRoi.width / divs;
	int pixelHeight = containedRoi.height / divs;

	Size pixelSize = Size(pixelWidth, pixelHeight);
	Rect pixelRect = Rect(containedRoi.x, containedRoi.y, pixelWidth, pixelHeight);

	for (int i = 0; i < divs; i++)
		for (int j = 0; j < divs; j++)
		{
			pixelRect.x = containedRoi.x + i * pixelWidth;
			pixelRect.y = containedRoi.y + j * pixelHeight;
			

			Mat pixel = newMat(pixelRect), blurredPixel;
			blur(pixel, blurredPixel, pixelSize);

			blurredPixel.copyTo(pixel);
		}

	return newMat;
}