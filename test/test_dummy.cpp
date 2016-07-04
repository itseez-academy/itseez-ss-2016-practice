#include <gtest/gtest.h>

#include "opencv2/core/core.hpp"
#include "workaround.hpp"
#include <iostream>

using namespace cv;

TEST(dummy, dummy_test)
{
    Mat mat(3, 7, CV_8UC1, Scalar::all(0));
    Mat submat = mat(Rect(0, 0, 2, 3));
    EXPECT_FALSE(submat.isContinuous());
}

TEST(Threshold, can_threshold_1_elem_when_it_needs)
{
	unsigned char* data = new unsigned char[1];
	data[0] = 126;
	MatrixProcessor processor;
	processor.Threshold(data, 1, 1, 128);
	EXPECT_EQ(data[0], 0);
}

TEST(Threshold, dont_threshold_1_elem_when_it_dont_need)
{
	unsigned char* data = new unsigned char[1];
	data[0] = 129;
	MatrixProcessor processor;
	processor.Threshold(data, 1, 1, 128);
	EXPECT_EQ(data[0], 129);
}

TEST(Threshold, work_correctly_with_many_elements)
{
	unsigned char* data = new unsigned char[255];
	for ( int i = 0; i < 255; i++ )
		data[i] = i;
	MatrixProcessor processor;
	processor.Threshold(data, 16, 16, 128);

	int flag = 0;
	for (int i = 0; i < 255; i++)
		if ((data[i] < 128) && (data[i]))
			flag = 1;
		else if ((data[i] >= 128) && (data[i]) != i)
			flag = 1;
	EXPECT_FALSE(flag);
}