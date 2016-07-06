#include <gtest/gtest.h>

#include "opencv2/core/core.hpp"

#include <iostream>

#include "C:\Users\iss2016\itseez-ss-2016-practice\include\workaround.hpp"

using namespace cv;

TEST(dummy, dummy_test)
{
    Mat mat(3, 7, CV_8UC1, Scalar::all(0));
    Mat submat = mat(Rect(0, 0, 2, 3));
    EXPECT_FALSE(submat.isContinuous());
}


