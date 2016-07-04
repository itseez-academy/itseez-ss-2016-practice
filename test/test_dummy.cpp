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

TEST(Threshold, throws_when_arguments_are_incorrect){
	unsigned char* incorrectData = NULL;
	MatrixProcessor proc;
	ASSERT_ANY_THROW(proc.Threshold(incorrectData, 1, 1, 1));

	unsigned char* correctData = new unsigned char [4];
	correctData[0] = 37;
	correctData[1] = 158;
	correctData[2] = 0;
	correctData[3] = 41;
	ASSERT_ANY_THROW(proc.Threshold(correctData, 1, 1, -1));
	ASSERT_ANY_THROW(proc.Threshold(correctData, -1, 1, 40));
	ASSERT_ANY_THROW(proc.Threshold(correctData, 1, -1, 40));
}

TEST(Threshold, Threshold_works_properly){
	const size_t size = 10;
	unsigned char* actual = new unsigned char [size];
	unsigned char* expected = new unsigned char [size];
	const int threshold = 150;
	#include <cstdlib>
	srand(100u);
	for (size_t i = 0; i < 10; i++){
		unsigned char tmp = rand() % 256;
		actual[i] = tmp;
		expected[i] = tmp < threshold ? 0 : tmp;
	}

	MatrixProcessor proc;
	proc.Threshold(actual, 2, 5, threshold);

	for(size_t i = 0; i < size; i++){
		EXPECT_EQ(expected[i], actual[i]);
	}
}