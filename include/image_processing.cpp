#pragma once

#include "image_processing.hpp"


cv::Mat ImageProcessor1::CvtColor(const cv::Mat &src, const cv::Rect &roi)
{
	Mat rez,tmp;
	src.copyTo(rez);
	tmp = rez(roi);
	Mat srcGray;
	cvtColor(tmp, srcGray, CV_RGB2GRAY);
	vector<Mat> channels;
	channels.push_back(srcGray);
	channels.push_back(srcGray);
	channels.push_back(srcGray);
	Mat dst_roi;
	merge(channels, dst_roi);
	dst_roi.copyTo(tmp);
	return rez;
}

cv::Mat ImageProcessor1::Filter(const cv::Mat &src, const cv::Rect &roi,
	const int kSize)
{
	Mat rez, tmp;
	src.copyTo(rez);
	tmp = rez(roi);
	medianBlur(tmp, tmp, 7);
	return rez;
}
cv::Mat ImageProcessor1::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
	const int filterSize, const int lowThreshold, const int ratio,
	const int kernelSize)
{
	Mat rez, tmp, srcroi;
	src.copyTo(rez);
	srcroi = src(roi);
	tmp = rez(roi);
	Mat gray,edges;
	cvtColor(tmp, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, gray, Size(7, 7), 1.5);
	Canny(gray, edges, 0, 50);
	tmp = Scalar::all(0);
	srcroi.copyTo(tmp, edges);
	return rez;
}
cv::Mat ImageProcessor1::Pixelize(const cv::Mat &src, const cv::Rect &roi,
	const int kDivs)
{
	Mat rez, tmp, srcroi;
	src.copyTo(rez);
	tmp = rez(roi);
	Rect r;
	Mat block;
	int block_size_x = roi.width / kDivs;
	int block_size_y = roi.height / kDivs;
	for (int y = 0; y < roi.height - block_size_y; y += block_size_y)
		for (int x = 0; x < roi.width - block_size_x; x += block_size_x)
		{
			r = Rect(x, y, block_size_x, block_size_y);
			block = tmp(r);
			blur(block, block, Size(block_size_x, block_size_y));
		}
	return rez;
}