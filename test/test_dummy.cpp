#include <gtest/gtest.h>

#include "opencv2/core/core.hpp"

#include <iostream>
#include"workaround.hpp"

using namespace cv;

TEST(dummy, dummy_test)
{
    Mat mat(3, 7, CV_8UC1, Scalar::all(0));
    Mat submat = mat(Rect(0, 0, 2, 3));
    EXPECT_FALSE(submat.isContinuous());
}
TEST(threshold,test1)
{
	MatrixProcessor mp;
	unsigned char data = 50;
	mp.Threshold(&data,1,1,51);
	EXPECT_EQ(data, 0);
}
TEST(threshold, test2)
{
	MatrixProcessor mp;
	unsigned char  data[4] = {50,62,63,50};
	mp.Threshold(data, 2, 2, 51);
	unsigned char actual[4] = {0,62,63,0};
	
	bool f = 0;
	for (size_t  i = 0; i < 4; i++)
	{
		if (data[i] != actual[i])
			f = 1;
	}
	EXPECT_EQ(f,0);
}
TEST(averaging,test1)
{

	MatrixProcessor mp;
	unsigned char  data[16] = { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5 };
	mp.Averaging(data, 4, 4, 1);
	unsigned char actual[16] = { 2,3,3,2,3,15,15,3,3,15,15,3,2,3,3,2 };

	bool f = 0;
	for (size_t i = 0; i < 16; i++)
	{
		if (data[i] != actual[i])
			f = 1;
	}
	EXPECT_EQ(f, 0);
}