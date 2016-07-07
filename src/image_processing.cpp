#include "image_processing.hpp"

#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

Mat ImageProcessorImpl::CvtColor(const Mat &src, const Rect &roi) {
  Mat src_copy;
  src.copyTo(src_copy);

  Mat src_roi = src_copy(roi), dst_gray_roi;
  cvtColor(src_roi, dst_gray_roi, CV_BGR2GRAY);

  vector<Mat> channels;
  channels.push_back(dst_gray_roi);
  channels.push_back(dst_gray_roi);
  channels.push_back(dst_gray_roi);
  Mat dst_roi;
  merge(channels, dst_roi);

  dst_roi.copyTo(src_roi);

  return src_copy;
}

Mat ImageProcessorImpl::Filter(const Mat &src, const Rect &roi,
                               const int size) {
  Mat src_copy;
  src.copyTo(src_copy);

  Mat src_roi = src_copy(roi);
  blur(src_roi, src_roi, Size(size, size));

  return src_copy;
}

Mat ImageProcessorImpl::DetectEdges(const Mat &src, const Rect &roi,
                                    const int size, const int low_threshold,
                                    const int ratio, const int kernel_size) {
  Mat src_roi = src(roi), src_gray_roi;
  cvtColor(src_roi, src_gray_roi, CV_BGR2GRAY);

  Mat gray_blurred;

  blur(src_gray_roi, gray_blurred, Size(size, size));

  Mat detected_edges;
  Canny(gray_blurred, detected_edges, low_threshold, low_threshold * ratio,
        kernel_size);

  Mat dst;
  src.copyTo(dst);
  Mat dst_roi = dst(roi);
  dst_roi = Scalar::all(0);
  src_roi.copyTo(dst_roi, detected_edges);

  return dst;
}

Mat ImageProcessorImpl::Pixelize(const Mat &src, const Rect &roi,
                                 const int divs) {
  Mat src_copy;
  src.copyTo(src_copy);
  Mat src_copy_roi = src_copy(roi);

  int block_size_x = roi.width / divs, block_size_y = roi.height / divs;
  for (int x = 0; x < roi.width - block_size_x; x += block_size_x) {
    for (int y = 0; y < roi.height - block_size_y; y += block_size_y) {
      Mat src_roi_block = src_copy_roi(Rect(x, y, block_size_x, block_size_y));
      blur(src_roi_block, src_roi_block, Size(block_size_x, block_size_y));
    }
  }

  return src_copy;
}
