#include <gtest/gtest.h>

#include "opencv2/core/core.hpp"

#include <iostream>

#include "workaround.hpp"

using namespace cv;

TEST(dummy, dummy_test)
{
    Mat mat(3, 7, CV_8UC1, Scalar::all(0));
    Mat submat = mat(Rect(0, 0, 2, 3));
    EXPECT_FALSE(submat.isContinuous());
}


TEST(dummy, all_less_than_thrash)
{
  MatrixProcessor processor;
  unsigned char* data = new unsigned char[6];
  for (int i = 0; i < 6; i++)
    data[i] = 6;
  processor.Threshold(data, 3, 2, 10);
  int FLAG = 1;
  for (int i = 0; i < 6; i++)
    if (data[i] != 0)
      FLAG = 0;
  ASSERT_TRUE(FLAG == 1);
}

TEST(dummy, one_less_than_thrash)
{
  MatrixProcessor processor;
  unsigned char* data = new unsigned char[2];
  data[0] = 5;
  data[1] = 10;
  processor.Threshold(data, 1, 2, 10);
  ASSERT_TRUE(data[0] == 0 && data[1] == 10);
}