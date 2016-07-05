#include "image_processing.hpp"
#include "workaround.hpp"

#include <opencv2\imgproc.hpp>

using namespace cv;

Mat ImageProcessorImpl::CvtColor(const cv::Mat &src, const cv::Rect &roi)
{
	Mat newMat = src.clone();

	Mat roiMat = newMat(roi);
	Mat grayRoi;

	cvtColor(roiMat, grayRoi, CV_RGB2GRAY, 1);

	Mat tmp[3];
	tmp[0] = tmp[2] = tmp[3] = grayRoi;

	merge(tmp, 3, roiMat);

	return newMat;
}

Mat ImageProcessorImpl::Filter(const cv::Mat &src, const cv::Rect &roi, const int size)
{
	int actualSize = size % 2 == 0 ? max(size - 1, 1) : size;

	MatrixProcessor mp;

	Mat newMat = src.clone();

	Mat roiMat = newMat(roi);
	Mat dataMat = roiMat.clone();

	mp.Median(dataMat.data, roi.width, roi.height, size / 2);
	merge(&dataMat, 1, roiMat);

	return newMat;
}

Mat ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
	const int filter_size, const int low_threshold,
	const int ratio, const int kernel_size)
{

}

Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi, const int divs)
{

}