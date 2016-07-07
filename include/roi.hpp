#pragma once

#include "opencv2/core/core.hpp"

cv::Rect getContainedRoi(const cv::Mat & src, const cv::Rect & roi);