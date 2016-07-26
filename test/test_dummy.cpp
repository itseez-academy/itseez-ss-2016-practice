#include <gtest/gtest.h>
#include "workaround.hpp"
#include "opencv2/core/core.hpp"

#include <iostream>
#define Size1 6 
const uchar rez[Size1] = { 0,128,228,0,148,0 };

using namespace cv;

TEST(dummy, dummy_test)
{
    //Mat mat(3, 7, CV_8UC1, Scalar::all(0));
	MatrixProcessor ap;
	uchar* mas = new uchar[Size1];
	mas[0] = 34;
	mas[1] = 128;
	mas[2] = 228;
	mas[3] = 28;
	mas[4] = 148;
	mas[5] = 8;
	ap.Threshold(mas, 2, 3, 128);
	bool flag = false;
	for (int i = 0; i < Size1; ++i) 
	{
		//EXPECT_EQ(mas[i], rez[i]) << "Vectors x and y differ at index " << i;
		if (mas[i] != rez[i])
			flag = true;
	}
    //Mat submat = mat(Rect(0, 0, 2, 3));
    EXPECT_FALSE(flag);
	delete []mas;
}
