#include <gtest/gtest.h>

#include "workaround.hpp"

#include <iostream>
using namespace std;


TEST(picture, picture_test)
{
	unsigned char data[1] = { 5 };
	//unsigned char* const dataptr = &data[0];
	MatrixProcessor first_picture;
	first_picture.MatrixProcessor::Threshold(data,1,1,6);
	EXPECT_EQ(0,data[0]);


}