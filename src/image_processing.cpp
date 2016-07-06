#include "image_processing.hpp"

cv::Mat ImageProcessorImpl::CvtColor(const cv::Mat &src, const cv::Rect &roi) {

	cv::Mat srcCopy = src.clone();

	cv::Mat srcCopyRoi(srcCopy(roi));
	cv::Mat dstGrayRoi;

	cv::cvtColor(srcCopyRoi, dstGrayRoi, cv::COLOR_BGR2GRAY);

	std::vector<cv::Mat> channels;
	channels.push_back(dstGrayRoi);
	channels.push_back(dstGrayRoi);
	channels.push_back(dstGrayRoi);

	cv::Mat dstRoi;
	cv::merge(channels, dstRoi);
	dstRoi.copyTo(srcCopyRoi);

	return srcCopy;
}

cv::Mat ImageProcessorImpl::Filter(const cv::Mat &src, const cv::Rect &roi,
		const int kSize) {

	cv::Mat srcCopy;
	src.copyTo(srcCopy);

	cv::Mat srcCopyRoi = srcCopy(roi);
	cv::medianBlur(srcCopyRoi, srcCopyRoi, kSize);

	return srcCopy;
}

cv::Mat ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
		const int filterSize, const int lowThreshold, const int ratio, const int kernelSize) {


	cv::Mat srcRoi = src(roi);

	cv::Mat srcGrayRoi;
	cv::cvtColor(srcRoi, srcGrayRoi, cv::COLOR_BGR2GRAY);

	cv::Mat grayBlurred;
	cv::blur(srcGrayRoi, grayBlurred, cv::Size(filterSize, filterSize));
	
	cv::Mat detectedEdges;
	cv::Canny(grayBlurred, detectedEdges, lowThreshold, lowThreshold * ratio, kernelSize);

	cv::Mat dst;
	src.copyTo(dst);
	cv::Mat dstRoi = dst(roi);
	dstRoi = cv::Scalar::all(0);

	srcRoi.copyTo(dstRoi, detectedEdges);
	return dst;
}

cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi,
		const int kDivs) {

	cv::Mat srcCopy;
	src.copyTo(srcCopy);
	cv::Mat srcCopyRoi = srcCopy(roi);
	const int	blockSize_X = roi.width / kDivs,
				blockSize_Y = roi.height / kDivs;
	for (int x = 0; x < srcCopyRoi.cols - blockSize_X; x += blockSize_X) {
		for (int y = 0; y < srcCopyRoi.rows - blockSize_Y; y += blockSize_Y) {
			cv::Mat srcRoiBlock = srcCopyRoi(cv::Rect(x, y, blockSize_X, blockSize_Y));
			cv::blur(srcRoiBlock, srcRoiBlock, cv::Size(blockSize_X, blockSize_Y));
		}
	}
	return srcCopy;
}
