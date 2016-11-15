#include <gtest/gtest.h>

#include "workaround.hpp"

TEST(Threshold, No_Throw_When_DataOK) {
  MatrixProcessor mp;
  int width = 4, height = 4;
  unsigned char* data;
  unsigned char threshold = 5;

  data = new unsigned char[width * height];
  for (int i = 0; i < width * height; i++)
    data[i] = i;

  EXPECT_NO_THROW(mp.Threshold(data, width, height, threshold));
}

TEST(Threshold, Correct_Work) {
  MatrixProcessor mp;
  int width = 4, height = 4;
  unsigned char* data;
  unsigned char threshold = 5;

  data = new unsigned char[width * height];
  for (int i = 0; i < width * height; i++)
    data[i] = i;

  mp.Threshold(data, width, height, threshold);
  EXPECT_EQ(0, data[0]);
}

TEST(Threshold, Correct_Work_With_One_Element) {
  MatrixProcessor mp;
  int width = 1, height = 1;
  unsigned char* data;
  unsigned char threshold = 6;

  data = new unsigned char[width * height];
  data[0] = threshold + 1;

  mp.Threshold(data, width, height, threshold);
  EXPECT_EQ(threshold + 1, data[0]);
}

TEST(Averaging, Correct_Work) {
  MatrixProcessor mp;
  const int width = 3, height = 3;
  unsigned char* data;
  int surroundings = 1;

  data = new unsigned char[width * height];
  for (int i = 0; i < width * height; i++)
    data[i] = i;

  unsigned char expected[width * height] = { 2,2,3,3,4,4,5,5,6 };
  mp.Averaging(data, width, height,surroundings);

  for (int i = 0; i < width * height; i++)
    EXPECT_EQ(expected[i], data[i]);
}

TEST(Averaging, Correct_Work_With_Big_Surroudings) {
  MatrixProcessor mp;
  const int width = 5, height = 5;
  unsigned char* data;
  int surroundings = 4;
  int expected = 0;

  data = new unsigned char[width * height];
  for (int i = 0; i < width * height; i++) {
    data[i] = i;
    expected += i;
  }

  expected /= width * height;
  mp.Averaging(data, width, height, surroundings);

  EXPECT_EQ(expected, data[0]);
}

TEST(MefianFilter, Correct_Work) {
  MatrixProcessor mp;
  const int width = 3, height = 3;
  unsigned char* data;
  int surroundings = 1;

  data = new unsigned char[width * height];
  for (int i = 0; i < width * height; i++)
    data[i] = i;

  unsigned char expected[width * height] = { 2,2,3,3,4,4,5,5,6 };
  mp.MedianFilter(data, width, height,surroundings);

  for (int i = 0; i < width * height; i++)
    EXPECT_EQ(expected[i], data[i]);
}
