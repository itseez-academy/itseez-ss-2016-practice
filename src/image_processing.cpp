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

Mat ImageProcessorImpl::Filter(const Mat &src, const Rect &roi, 
    const int kSize) {
  Mat srcCopy;
  src.copyTo(srcCopy);

  Mat srcRoi = srcCopy(roi);
  blur(srcRoi, srcRoi, Size(kSize, kSize));

  return srcCopy;
}

Mat ImageProcessorImpl::DetectEdges(const Mat &src, const Rect &roi, 
    const int kSize, const int lowThreshold, const int ratio, 
    const int kernelSize) {
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

Mat ImageProcessorImpl::Pixelize(const Mat &src, const Rect &roi, 
    const int kDivs) {
  Mat srcCopy;
  src.copyTo(srcCopy);
  Mat srcCopyRoi = srcCopy(roi);

  int blockSizeX = roi.width / kDivs, blockSizeY = roi.height / kDivs;
  for (int x = 0; x < roi.width - blockSizeX; x += blockSizeX) {
    for (int y = 0; y < roi.height - blockSizeY; y += blockSizeY) {
      Mat srcRoiBlock = srcCopyRoi(Rect(x, y, blockSizeX, blockSizeY));
      blur(srcRoiBlock, srcRoiBlock, Size(blockSizeX, blockSizeY));
    }
  }

  return srcCopy;
}