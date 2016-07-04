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

void testWithT(int t)
{
	int width = 3, height = 3;

	unsigned char * data = new unsigned char[9];
	unsigned char * expected = new unsigned char[9];

	MatrixProcessor mp;

	for (int i = 0; i < 9; i++)
	{
		data[i] = i;
		expected[i] = (i < t) ? 0 : i;
	}

	mp.Threshold(data, width, height, t);

	bool correct = true;

	for (int i = 0; i < 9; i++)
		if (data[i] != expected[i])
			correct = false;
	
	EXPECT_TRUE(correct);

	delete[] data;
	delete[] expected;
}

TEST(threshold, threshold_test_right)
{
	testWithT(9);
}

TEST(threshold, threshold_test_left)
{
	testWithT(0);
}

TEST(threshold, threshold_test_middle)
{
	testWithT(4);
}