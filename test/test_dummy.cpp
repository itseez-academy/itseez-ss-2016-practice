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
  const int size = 6;
  const int thresh = 10;
  const int width = 3, height = 2;
  MatrixProcessor processor;
  unsigned char* data = new unsigned char[size];
  for (int i = 0; i < size; i++)
    data[i] = size;

  processor.Threshold(data, width, height, thresh);

  int FLAG = 1;
  for (int i = 0; i < size; i++)
    if (data[i] != 0)
      FLAG = 0;

  ASSERT_TRUE(FLAG == 1);
}

TEST(dummy, one_less_than_thrash)
{
  const int size = 2;
  const int thresh = 10;
  const int width = 2, height = 1;
  MatrixProcessor processor;
  unsigned char* data = new unsigned char[size];
  data[0] = 5;
  data[1] = 10;

  processor.Threshold(data, width, height, thresh);

  ASSERT_TRUE(data[0] == 0 && data[1] == 10);
}

TEST(dummy, avg_from_one)
{
  const int size = 1;
  const int radius = 3;
  unsigned char* data = new unsigned char[size];
  data[0] = 5;
  MatrixProcessor processor;
  processor.Average(data, 1, 1, radius);
  ASSERT_TRUE(data[0] == 5);
}

TEST(dummy, avg_then_all_equals)
{
  const int size = 81;
  const int radius = 5;
  unsigned char* data = new unsigned char[size];
  for (int i = 0; i < size; i++)
    data[i] = 10;
  MatrixProcessor processor;
  processor.Average(data, 9, 9, radius);
  int flag = 1;
  for (int i = 0; i < size; i++)
    if (data[i] != 10)
    {
      flag = 0;
      break;
    }
  ASSERT_TRUE(flag == 1);
}