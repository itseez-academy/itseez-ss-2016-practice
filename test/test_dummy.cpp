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

TEST(Average, MatrixProcessor_test)
{
	MatrixProcessor processor;
	unsigned char* data = new unsigned char[9] { 1, 9, 1, 1, 1, 1, 1, 2, 1 };

	processor.Average(data, 3, 3);
	ASSERT_TRUE(data[4] == 2);
}

