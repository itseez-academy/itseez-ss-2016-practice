#include <gtest/gtest.h>
#include "workaround.hpp"

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
	unsigned char tmp[3][3];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; i++)
			tmp[i][j] = 1;
	tmp[1][1] = 10;

	 
}
