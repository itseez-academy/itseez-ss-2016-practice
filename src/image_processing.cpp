#include "image_processing.hpp"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

#include <vector>

using namespace cv;
using namespace std;

Mat ImageProcessorImpl::CvtColor(const Mat &src, const Rect &roi) {
  Mat srcCopy;
  src.copyTo(srcCopy);
  
  Mat srcRoi = srcCopy(roi), dstGrayRoi;
  cvtColor(srcRoi, dstGrayRoi, CV_BGR2GRAY);

  vector<Mat> channels;
  channels.push_back(dstGrayRoi);
  channels.push_back(dstGrayRoi);
  channels.push_back(dstGrayRoi);
  Mat dstRoi;
  merge(channels, dstRoi);

  dstRoi.copyTo(srcRoi);

  return srcCopy;
}

Mat ImageProcessorImpl::Filter(const Mat &src, const Rect &roi, int kSize) {
  Mat srcCopy;
  src.copyTo(srcCopy);

  Mat srcRoi = srcCopy(roi);
  blur(srcRoi, srcRoi, Size(kSize, kSize));

  return srcCopy;
}

Mat ImageProcessorImpl::DetectEdges(const Mat &src, const Rect &roi, int kSize,
  int lowThreshold, int ratio, int kernelSize) {
  Mat srcRoi = src(roi), srcGrayRoi;
  cvtColor(srcRoi, srcGrayRoi, CV_BGR2GRAY);

  Mat grayBlurred;
  
  blur(srcGrayRoi, grayBlurred, Size(kSize, kSize));

  Mat detectedEdges;
  Canny(grayBlurred, detectedEdges, lowThreshold,
    lowThreshold * ratio, kernelSize);

  Mat dst;
  src.copyTo(dst);
  Mat dstRoi = dst(roi);
  dstRoi = Scalar::all(0);
  srcRoi.copyTo(dstRoi, detectedEdges);

  return dst;
}

cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi) {
  throw std::exception("Pixelize method is not implemented");
}