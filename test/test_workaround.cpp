#include <gtest/gtest.h>
#include "string"

#include "workaround.hpp"

TEST(MatrixProcessor, Threshold_No_Throw_When_DataOK) {
    MatrixProcessor mp;
    int width = 4, height = 4;
    unsigned char* data;
    unsigned char threshold = 5;

    data = new unsigned char[width * height];
    for (int i = 0; i < width * height; i++)
        data[i] = i;

    EXPECT_NO_THROW(mp.Threshold(data, width, height, threshold));
}

TEST(MatrixProcessor, Threshold_Correct_Work) {
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

TEST(MatrixProcessor, Threshold_Correct_Work_With_One_Element) {
    MatrixProcessor mp;
    int width = 1, height = 1;
    unsigned char* data;
    unsigned char threshold = 6;

    data = new unsigned char[width * height];
    data[0] = threshold + 1;

    mp.Threshold(data, width, height, threshold);
    EXPECT_EQ(threshold + 1, data[0]);
}

TEST(MatrixProcessor, Averaging_Correct_Work) {
    MatrixProcessor mp;
    const int width = 3, height = 3;
    unsigned char* data;
    int surroundings = 1;

    data = new unsigned char[width * height];
    for (int i = 0; i < width * height; i++)
        data[i] = i;

    unsigned char expected[width * height] = { '2','2','3','3','4','4','5','5','6' };
    mp.Averaging(data, width, height,surroundings);
    bool flag = true;

    std::string str;
    for (int i = 0; i < width * height; i++)
        str[i] = data[i];

    for (int i = 0; i < width * height; i++)
        if (data[i] != expected[i])
            flag = false;

    EXPECT_TRUE(flag);
}
