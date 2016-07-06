#include <gtest/gtest.h>

#include "opencv2/core/core.hpp"

#include "workaround.hpp"

#include <iostream>

using namespace cv;

TEST(dummy, dummy_test)
{
//    Mat mat(3, 7, CV_8UC1, Scalar::all(0));
//    Mat submat = mat(Rect(0, 0, 2, 3));
//   EXPECT_FALSE(submat.isContinuous());

//	unsigned char* data;

	int const width1 = 1;
	int const height1 = 100;

	int const width2 = 100;
	int const height2 = 1;

	int const width3 = 1;
	int const height3 = 1;

	int threshold1 = 512;
	int threshold2 = 0;
	int threshold3 = -512;

	unsigned char testArr1[width1*height1];
	unsigned char testArr2[width2*height2];
	unsigned char testArr3[width3*height3];

	for (int i = 0; i < width1*height1; i++) 
	{
		if (i < (width1*height1 / 2))
			testArr1[i] = threshold1 - 1;
		else
			testArr1[i] = threshold1 + 1;
	}

	for (int i = 0; i < width2*height2; i++)
	{
		if (i < (width2*height2 / 2))
			testArr2[i] = threshold2 - 1;
		else
			testArr2[i] = threshold2 + 1;
	}

	for (int i = 0; i < width3*height3; i++)
	{
		if (i < (width3*height3 / 2))
			testArr3[i] = threshold3 - 1;
		else
			testArr3[i] = threshold3 + 1;
	}

	
	MatrixProcessor processor;
	processor.Threshold(testArr1, width1, height1, threshold1);

	//Threshold(unsigned char* const data, const int width,
	//	const int height, const int threshold);


    Mat mat(3, 7, CV_8UC1, Scalar::all(0));
    Mat submat = mat(Rect(0, 0, 2, 3));
    EXPECT_FALSE(submat.isContinuous());
}
