#include <gtest/gtest.h>

#include "opencv2/core/core.hpp"

#include <iostream>
#include "workaround.hpp"

using namespace cv;

TEST(dummy, dummy_test)
{
    Mat mat(3, 7, CV_8UC1, Scalar::all(0));
    Mat submat = mat(Rect(0, 0, 2, 3));
    EXPECT_FALSE(submat.isContinuous());
}

TEST(threshold, treshold_test)
{
	Mat mat(3, 3, CV_8UC1, Scalar::all(125));
	Mat zeros(3, 3, CV_8UC1, Scalar::all(0));
	MatrixProcessor processor;
	processor.Threshold(mat.data, mat.cols, mat.rows, 128);
	EXPECT_TRUE(cv::norm(mat.reshape(1, 1), cv::NORM_L1) == 0.0);
}