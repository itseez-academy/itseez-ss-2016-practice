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