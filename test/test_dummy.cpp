#include <gtest/gtest.h>

#include "opencv2/core/core.hpp"
#include <image_processing.hpp>

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv/cv.hpp>

using namespace cv;

TEST(dummy, dummy_test)
{
    Mat mat(3, 7, CV_8UC1, Scalar::all(0));
    Mat submat = mat(Rect(0, 0, 2, 3));
    EXPECT_FALSE(submat.isContinuous());
}

TEST(ImageProcessorImpl, CvtColor) {
    ImageProcessorImpl processor;
    cv::Mat image        = cv::imread("/home/tolik/Downloads/mops.jpg");
    cv::Mat processImage = processor.DetectEdges(image, cv::Rect(100,50,200,70), 5, 5, 5, 5);
    cv::imshow("test window", processImage);
    cv::waitKey(0);
}
