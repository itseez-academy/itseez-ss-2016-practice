#include "image_processing.hpp"
#include "opencv2/opencv.hpp"
#include <cstddef>
using namespace cv;

Mat ImageProcessorImpl::CvtColor(const Mat &src, const Rect &roi)
{
	Mat src_copy = src.clone();
	Mat src_copy_roi = src_copy(roi);
	Mat dst_roi = src_copy_roi.clone(), dst_gray_roi;
	std::vector<Mat> channels;

	cvtColor(src_copy_roi, dst_gray_roi, COLOR_BGR2GRAY); // RGB или BGR или ... ?
	
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);
	channels.push_back(dst_gray_roi);
	
	merge(channels, dst_roi);
	dst_roi.copyTo(src_copy_roi); // difference between copyTo and clone, src_copy_roi = dst_roi.clone() doesnt work
	return src_copy;
}

cv::Mat ImageProcessorImpl::Filter(const Mat &src, const Rect &roi, const int size)
{	
	Mat src_copy = src.clone(); 
	Mat src_copy_roi = src_copy(roi);
	
	medianBlur(src_copy_roi,src_copy_roi,size);
	
	return src_copy;
	
}
// для чего нужны аргументы? filterSize, ratio, kernelSize;
Mat ImageProcessorImpl::DetectEdges(const Mat &src, const Rect &roi,
	const int filterSize, const int lowThreshold, const int ratio,
	const int kernelSize)
{
	Mat src_copy = src.clone();
	Mat src_copy_roi = src_copy(roi);
	Mat src_gray_roi , gray_blurred, detected_edges, dst, dst_roi;
	Size a(kernelSize,kernelSize);
	
	cvtColor(src_copy_roi, src_gray_roi, COLOR_BGR2GRAY);
	blur(src_gray_roi, gray_blurred,a); // ? зачем фильтровать?
	Canny(gray_blurred, detected_edges, lowThreshold, lowThreshold*ratio);
	
	dst = src.clone();
	dst_roi = dst(roi);
	dst_roi = Scalar::all(0);
	src_copy_roi.copyTo(dst_roi, detected_edges);
	return dst; // зачем обнулять? зачем копировать? почему нельзя вернуть сраpу detected_edges
}

//как нумеруются элементы в подматрице?
cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat & src, const cv::Rect & roi, const int divs)
{
	Mat src_copy = src.clone(); // отличия clone от copyTo
	Mat src_copy_roi = src_copy(roi);
	int block_size_x = (int)(roi.width / divs), block_size_y = (int)(roi.height / divs);
	Size kernSize(block_size_x, block_size_y);

	for(int x = 0; x < src_copy_roi.cols - block_size_x; x += block_size_x)  // 0 или roi.x?
		for (int y = 0; y < src_copy_roi.rows - block_size_y ; y += block_size_y) // 0 или roi.y
		{	
			//if (roi.contains(Point(x + 1, y + 1)) && roi.contains(Point(x , y + 1)) && roi.contains(Point(x + 1, y)) && roi.contains(Point(x , y)))
				Mat src_roi_block = src_copy_roi(Rect(x,y,block_size_x,block_size_y));
				blur(src_roi_block, src_roi_block, kernSize);
			
		}

	return src_copy;
}