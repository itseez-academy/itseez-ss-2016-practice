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
	unsigned char* data = new unsigned char[256];
	for ( int i = 0; i < 256; i++ )
		data[i] = i;
	MatrixProcessor processor;
	processor.Threshold(data, 16, 16, treshold);

	int flag = 0;
	for (int i = 0; i < 256; i++)
		if ((data[i] < treshold) && (data[i]))
			flag = 1;
		else if ((data[i] >= treshold) && (data[i]) != i)
			flag = 1;
	EXPECT_FALSE(flag);
}

TEST(Averaging, dont_change_monotone_img)
{
	const int width = 16;
	const int height = 16;
	const unsigned char tint = 128;
	const int surroundings = 10;
	unsigned char* data = new unsigned char[width * height];
	for (int i = 0; i < width * height; i++)
		data[i] = tint;
	MatrixProcessor processor;
	processor.Averaging(data, width, height, surroundings);
	
	int flag = 0;
	for (int i = 0; i < width * height; i++)
		if (data[i] != tint)
			flag = 1;

	EXPECT_FALSE(flag);
}

TEST(Averaging, it_works_when_surroundings_is_too_large)
{
	const int width = 5;
	const int height = 5;
	const unsigned char tint = 128;
	const int surroundings = 3;
	unsigned char* data = new unsigned char[width * height];
	for (int i = 0; i < width * height; i++)
		data[i] = tint;
	MatrixProcessor processor;
	ASSERT_NO_THROW(processor.Averaging(data, width, height, surroundings));
}

TEST(Averaging, algorithm_works_correctly_on_central_element)
{
	const int width = 3;
	const int height = 3;
	const int surroundings = 1;
	unsigned char* data = new unsigned char[width * height];
	for (int i = 0; i < width * height; i++)
		data[i] = i;
	MatrixProcessor processor;
	processor.Averaging(data, width, height, surroundings);

	EXPECT_EQ(data[4], 4);
}

TEST(Averaging, algorithm_works_correctly_on_angular_element)
{
	const int width = 3;
	const int height = 3;
	const int surroundings = 1;
	unsigned char* data = new unsigned char[width * height];
	for (int i = 0; i < width * height; i++)
		data[i] = i;
	MatrixProcessor processor;
	processor.Averaging(data, width, height, surroundings);

	EXPECT_EQ(data[8], 6);
}