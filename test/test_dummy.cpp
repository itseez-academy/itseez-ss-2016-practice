#include <gtest/gtest.h>

#include "opencv2/core/core.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

#include "workaround.hpp"

using namespace cv;

/*
TEST(dummy, dummy_test)
{
    Mat mat(3, 7, CV_8UC1, Scalar::all(0));
    Mat submat = mat(Rect(0, 0, 2, 3));
    EXPECT_FALSE(submat.isContinuous());
}
*/

TEST(threshold_case1, threshold_test)
{

	unsigned char cmd1[] = { 0x30, 0x23, 0x40, 0x3 };
	unsigned char cmd2[] = { 0x30, 0x00, 0x40, 0x00 };
	MatrixProcessor processor;
	processor.Threshold(cmd1, 2, 2, 24);
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			unsigned char a = cmd1[i, j];
			unsigned char b = cmd1[i, j];

			EXPECT_EQ(a, b);
		}
	}
}
