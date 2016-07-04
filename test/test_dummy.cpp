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

void expectSameArrayContent(uchar * arr1, uchar * arr2, int size)
{
	bool correct = true;

	for (int i = 0; i < size; i++)
		if (arr1[i] != arr2[i])
			correct = false;

	EXPECT_TRUE(correct);
}

void testThreshold(int t)
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

	expectSameArrayContent(data, expected, width * height);

	delete[] data;
	delete[] expected;
}

TEST(threshold, threshold_test_right)
{
	testThreshold(9);
}

TEST(threshold, threshold_test_left)
{
	testThreshold(0);
}

TEST(threshold, threshold_test_middle)
{
	testThreshold(4);
}

TEST(average, average_small_test)
{
	/*
	mask size = 3x3

	000    000
	010 -> 000
	000    000

	*/

	int width = 3, height = 3, range = 1;

	unsigned char * data = new unsigned char[9];
	unsigned char * expected = new unsigned char[9];

	MatrixProcessor mp;

	expected[4] = 0;
	data[4] = 1;

	for (int i = 0; i < 3; i++)
	{
		data[i] = 0;
		data[width * i] = 0;
		data[width - 1 + width * i] = 0;
		data[height * width - (i + 1)] = 0;

		expected[i] = 0;
		expected[width * i] = 0;
		expected[width - 1 + width * i] = 0;
		expected[height * width - (i + 1)] = 0;
	}

	mp.Average(data, width, height, range);

	expectSameArrayContent(data, expected, width * height);

	delete[] data;
	delete[] expected;
}

TEST(average, average_big_test)
{
	/*
	mask size = 3x3

	0000    0000
	0120 -> 0110
	0340    0110
	0000    0000
	
	*/

	int width = 4, height = 4, range = 1;

	unsigned char * data = new unsigned char[16];
	unsigned char * expected = new unsigned char[16];

	MatrixProcessor mp;

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	data[5] = 1;
	data[6] = 2;
	data[7] = 0;
	data[8] = 0;
	data[9] = 3;
	data[10] = 4;
	data[11] = 0;
	data[12] = 0;
	data[13] = 0;
	data[14] = 0;
	data[15] = 0;

	expected[0] = 0;
	expected[1] = 0;
	expected[2] = 0;
	expected[3] = 0;
	expected[4] = 0;
	expected[5] = 1;
	expected[6] = 1;
	expected[7] = 0;
	expected[8] = 0;
	expected[9] = 1;
	expected[10] = 1;
	expected[11] = 0;
	expected[12] = 0;
	expected[13] = 0;
	expected[14] = 0;
	expected[15] = 0;

	mp.Average(data, width, height, range);

	expectSameArrayContent(data, expected, width * height);

	delete[] data;
	delete[] expected;
}

TEST(median, median_test)
{
	/*
	mask size = 3x3

	1000    1000
	0120 -> 0100
	0340    0120
	0302    0302

	*/

	int width = 4, height = 4, range = 1;

	unsigned char * data = new unsigned char[16];
	unsigned char * expected = new unsigned char[16];

	MatrixProcessor mp;

	data[0] = 1;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	data[5] = 1;
	data[6] = 2;
	data[7] = 0;
	data[8] = 0;
	data[9] = 3;
	data[10] = 4;
	data[11] = 0;
	data[12] = 0;
	data[13] = 3;
	data[14] = 0;
	data[15] = 2;

	expected[0] = 1;
	expected[1] = 0;
	expected[2] = 0;
	expected[3] = 0;
	expected[4] = 0;
	expected[5] = 1;
	expected[6] = 0;
	expected[7] = 0;
	expected[8] = 0;
	expected[9] = 1;
	expected[10] = 2;
	expected[11] = 0;
	expected[12] = 0;
	expected[13] = 3;
	expected[14] = 0;
	expected[15] = 2;

	mp.Median(data, width, height, range);

	expectSameArrayContent(data, expected, width * height);

	delete[] data;
	delete[] expected;
}