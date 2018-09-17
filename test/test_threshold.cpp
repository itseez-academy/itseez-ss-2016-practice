#include <gtest/gtest.h>
#include "workaround.hpp"
#include <iostream>
TEST(MatProcess, test_threshold)
{
	unsigned char tmp[] = { 4,5,1,1,1,5,7,7,4 };
	MatrixProcessor pic;

	pic.Threshold(tmp, 3, 3, 3);

	EXPECT_EQ(0, tmp[2]);
	EXPECT_EQ(0, tmp[3]);
	EXPECT_EQ(0, tmp[4]);
	
}

TEST(MatProcess, test_averaging)
{
	MatrixProcessor pic;
	unsigned char tmp[9];
	for (int i = 0; i < 9; i++)
			tmp[i]= 1;
	tmp[4] = 10;
	pic.AvrgElems(tmp, 3, 3, 1);
	EXPECT_EQ(3, tmp[0]);

	 
}
