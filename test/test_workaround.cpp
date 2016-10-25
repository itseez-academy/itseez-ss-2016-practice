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

/*TEST(MatrixProcessor, Throw_When_DataFail) {
    MatrixProcessor mp;
    int width = 4, height = 4;
    unsigned char* data;
    unsigned char threshold = 'e';

    EXPECT_ANY_THROW(mp.Threshold(data, width, height, threshold));
}*/

TEST(MatrixProcessor, Correct_Work) {
    MatrixProcessor mp;
    int width = 4, height = 4;
    unsigned char* data;
    unsigned char threshold = 'b';

    data = new unsigned char[width * height];
    for (int i = 0; i < width * height; i++)
        data[i] = 'a' + i;

    mp.Threshold(data, width, height, threshold);
    EXPECT_EQ(0, data[0]);
}

TEST(MatrixProcessor, Correct_Work_With_One_Element) {
    MatrixProcessor mp;
    int width = 1, height = 1;
    unsigned char* data;
    unsigned char threshold = 'b';

    data = new unsigned char[width * height];
    data[0] = threshold + 1;

    mp.Threshold(data, width, height, threshold);
    EXPECT_EQ(threshold + 1, data[0]);
}
