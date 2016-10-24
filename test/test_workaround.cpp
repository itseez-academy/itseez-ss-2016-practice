#include <gtest/gtest.h>

#include "workaround.hpp"

TEST(MatrixProcessor, No_Throw_When_DataOK) {
    MatrixProcessor mp;
    int width = 4, height = 4;
    unsigned char* data;
    unsigned char threshold = 'e';

    data = new unsigned char[width * height];
    for (int i = 0; i < width * height; i++)
        data[i] = static_cast<unsigned char>('a' + i);

    EXPECT_NO_THROW(mp.Threshold(data, width, height, threshold));
}

TEST(MatrixProcessor, Throw_When_DataFail) {
    MatrixProcessor mp;
    int width = 4, height = 4;
    unsigned char* data;
    unsigned char threshold = 'e';

    EXPECT_ANY_THROW(mp.Threshold(data, width, height, threshold));
}
