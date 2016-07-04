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
	const int treshold = 128;
	unsigned char* data = new unsigned char[1];
	data[0] = treshold - 1;
	MatrixProcessor processor;
	processor.Threshold(data, 1, 1, treshold);
	EXPECT_EQ(data[0], 0);
}

TEST(Threshold, dont_threshold_1_elem_when_it_dont_need)
{
	const int treshold = 128;
	unsigned char* data = new unsigned char[1];
	data[0] = treshold + 1;
	MatrixProcessor processor;
	processor.Threshold(data, 1, 1, treshold);
	EXPECT_EQ(data[0], treshold + 1);
}

TEST(Threshold, work_correctly_with_many_elements)
{
	const int treshold = 128;
	unsigned char* data = new unsigned char[255];
	for ( int i = 0; i < 255; i++ )
		data[i] = i;
	MatrixProcessor processor;
	processor.Threshold(data, 16, 16, treshold);

	int flag = 0;
	for (int i = 0; i < 255; i++)
		if ((data[i] < treshold) && (data[i]))
			flag = 1;
		else if ((data[i] >= treshold) && (data[i]) != i)
			flag = 1;
	EXPECT_FALSE(flag);
}