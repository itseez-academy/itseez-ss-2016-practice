#include "image_processing.hpp"
#include "opencv2/opencv.hpp"
#include <cstddef>
using namespace cv;
//?
Mat ImageProcessorImpl::CvtColor(const Mat &src, const Rect &roi)
{
	Mat src_copy = src;
	Mat src_copy_roi = src_copy(roi);
	Mat dst_gray_roi, dst_gray;
	Mat dst_roi = src_copy(roi);
	std::vector<Mat> channels;
	cvtColor(src_copy_roi, dst_gray_roi, COLOR_BGR2GRAY); // RGB или BGR или ... ?
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);
	merge(channels, dst_roi);
	src_copy_roi = dst_roi;
	return src_copy;
}

cv::Mat ImageProcessorImpl::Filter(const Mat &src, const Rect &roi, const int size)
{	
	Mat src_copy(src); //??
	Mat src_copy_roi = src_copy(roi);
	medianBlur(src_copy_roi,src_copy_roi,size);//
	return src_copy;
	//return cv::Mat(); ?
}
// для чего нужны аргументы? filterSize, ratio, kernelSize;
Mat ImageProcessorImpl::DetectEdges(const Mat &src, const Rect &roi,
	const int filterSize, const int lowThreshold, const int ratio,
	const int kernelSize)
{
	Mat src_copy(src);
	Mat src_copy_roi = src_copy(roi);
	Mat src_gray_roi;
	cvtColor(src_copy_roi, src_gray_roi, COLOR_BGR2GRAY);
	Mat gray_blurred; 
	Mat detected_edges;
	Size a;
	a.width = 1;
	a.height = 1;
	blur(src_gray_roi, gray_blurred,a); // ? зачем фильтровать?
	Canny(gray_blurred, detected_edges, 50, 200);
	Mat dst;
	src_copy.copyTo(dst);
	Mat dst_roi = dst(roi); // когда копируешь или присваиваешь они ссылаются все на один объект?
	dst_roi = Scalar::all(0);
	//dst_roi = src_copy_roi(detected_edges);
	//cvCopy(src_copy_roi, dst_roi, detected_edges);
	src_copy_roi.copyTo(dst_roi, detected_edges);
	return dst; // зачем обнулять? зачем копировать? почему нельзя вернуть сраpу detected_edges
}


cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat & src, const cv::Rect & roi, const int divs)
{
	Mat src_copy;
	src.copyTo(src_copy);
	Mat src_copy_roi = src_copy(roi);
	int block_size_x = roi.width / divs;
	int block_size_y = roi.height / divs;
	Size kernSize(block_size_x, block_size_y);
	Mat src_roi_block;
	for (int y = roi.y; y < roi.y+roi.height; y++)
		for (int x = roi.x; x < roi.x + roi.width; x++)
		{	
			Rect a(x, y, 0, 0);
			if (roi.contains(Point(x + 1, y + 1)) && roi.contains(Point(x , y + 1)) && roi.contains(Point(x + 1, y)) && roi.contains(Point(x , y)))
			{
				src_roi_block = src_copy_roi(a);
				blur(src_roi_block, src_copy_roi, kernSize);
			}
		}

	return src_copy;
}