#include <gtest/gtest.h>
#include <workaround.hpp>

#include "opencv2/core/core.hpp"

#include <iostream>

using namespace cv;

TEST(dummy, dummy_test)
{
    Mat mat(3, 7, CV_8UC1, Scalar::all(0));
    Mat submat = mat(Rect(0, 0, 2, 3));
    EXPECT_FALSE(submat.isContinuous());
}

TEST(threshold, threshold_test)
{
	int width = 3, height = 3;

	unsigned char * data = new unsigned char[9];
	MatrixProcessor mp;

	for (int t = 1; t < 10; t++)
	{
		for (int i = 0; i < 9; i++)
			data[i] = i + 1; //we have to reinitialize

		mp.Threshold(data, width, height, t);

		for (int i = 0; i < 9; i++)
			ASSERT_TRUE((data[i] == 0 && i + 1 < t) || (data[i] != 0 && i + 1 >= t));
	}

	delete[] data;
}