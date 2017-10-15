#include "image_processing.hpp"

cv::Mat ImageProcessorImpl::CvtColor(const cv::Mat &src,
                                     const cv::Rect &roi) {
    cv::Mat src_copy;
    src.copyTo(src_copy);
    cv::Mat src_copy_roi = src_copy(roi);
    cv::Mat dst_gray_roi;
    cv::cvtColor(src_copy_roi, dst_gray_roi, CV_BGR2GRAY);
    std::vector<cv::Mat> channels(3);
    std::fill(channels.begin(), channels.end(), dst_gray_roi );
    cv::Mat dst_roi;
    cv::merge(channels, dst_roi);
    dst_roi.copyTo(src_copy_roi);
    return src_copy;
}

cv::Mat ImageProcessorImpl::Filter(const cv::Mat &src,
                                   const cv::Rect &roi,
                                   const int kSize) {
    cv::Mat src_copy;
    src.copyTo(src_copy);
    cv::Mat src_copy_roi = src_copy(roi);
    cv::medianBlur(src_copy_roi, src_copy_roi, kSize);
    return src_copy;
}

cv::Mat
ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
                                const int filterSize,
                                const int lowThreshold,
                                const int ratio,
                                const int kernelSize) {

    cv::Mat src_roi = src(roi);
    cv::Mat src_gray_roi;
    cv::cvtColor(src_roi, src_gray_roi, CV_BGR2GRAY);
    cv::Mat gray_blured;
    cv::blur(src_gray_roi, gray_blured, cv::Size(filterSize, filterSize));
    cv::Mat detected_edges, dst;
    cv::Canny(gray_blured, detected_edges, lowThreshold, lowThreshold * ratio, kernelSize);
    src.copyTo(dst);
    cv::Mat dst_roi = dst(roi);
    dst_roi = cv::Scalar::all(0);
    src_roi.copyTo(dst_roi, detected_edges);
    return dst;
}

cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat &src,
                                     const cv::Rect &roi,
                                     const int kDivs) {
    cv::Mat src_copy;
    src.copyTo(src_copy);

    cv::Mat src_copy_roi = src_copy(roi);
    int x_block_size     = roi.width  / kDivs;
    int y_block_size     = roi.height / kDivs;

    for(int i = 0; i < src_copy_roi.rows; i += y_block_size )
        for(int j = 0; j < src_copy_roi.cols; j += x_block_size) {
            cv::Mat src_roi_block = src_copy_roi(cv::Rect(j, i, x_block_size, y_block_size));
            cv::blur(src_roi_block, src_roi_block, cv::Size(x_block_size, y_block_size));
        }
    return src_copy;
}



