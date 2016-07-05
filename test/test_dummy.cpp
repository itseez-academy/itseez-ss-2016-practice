#include <gtest/gtest.h>

#include "opencv2/core/core.hpp"
#include "workaround.hpp"
#include <iostream>

using namespace cv;

TEST(dummy, dummy_test){
    Mat mat(3, 7, CV_8UC1, Scalar::all(0));
    Mat submat = mat(Rect(0, 0, 2, 3));
    EXPECT_FALSE(submat.isContinuous());
}

TEST(Threshold, throws_when_arguments_are_incorrect){
	unsigned char* incorrectData = NULL;
	MatrixProcessor proc;
	ASSERT_ANY_THROW(proc.Threshold(incorrectData, 1, 1, 1));

	unsigned char* correctData = new unsigned char [4];
	correctData[0] = 37;
	correctData[1] = 158;
	correctData[2] = 0;
	correctData[3] = 41;
	ASSERT_ANY_THROW(proc.Threshold(correctData, 1, 1, -1));
	ASSERT_ANY_THROW(proc.Threshold(correctData, -1, 1, 40));
	ASSERT_ANY_THROW(proc.Threshold(correctData, 1, -1, 40));
	delete[]correctData;
}

TEST(Threshold, Threshold_works_properly){
	const size_t size = 10;
	unsigned char* actual = new unsigned char [size];
	unsigned char* expected = new unsigned char [size];
	const int threshold = 150;
	#include <cstdlib>
	srand(100u);
	for (size_t i = 0; i < 10; i++){
		unsigned char tmp = rand() % 256;
		actual[i] = tmp;
		expected[i] = tmp < threshold ? 0 : tmp;
	}

	MatrixProcessor proc;
	ASSERT_NO_THROW(proc.Threshold(actual, 2, 5, threshold));

	for(size_t i = 0; i < size; i++){
		EXPECT_EQ(expected[i], actual[i]);
	}
	delete[]actual;
	delete[]expected;
}

TEST(AVG, cant_proc_matrix_when_one_or_more_arguments_are_incorrect) {
	unsigned char* incorrectData = NULL;
	MatrixProcessor proc;
	ASSERT_ANY_THROW(proc.doAveragingOfElements(incorrectData, 1, 1, 1));

	unsigned char* correctData = new unsigned char[4];
	correctData[0] = 37;
	correctData[1] = 158;
	correctData[2] = 0;
	correctData[3] = 41;
	ASSERT_ANY_THROW(proc.doAveragingOfElements(correctData, 1, 1, -1));
	ASSERT_ANY_THROW(proc.doAveragingOfElements(correctData, -1, 1, 40));
	ASSERT_ANY_THROW(proc.doAveragingOfElements(correctData, 1, -1, 40));
	delete[]correctData;
}

TEST(AVG, can_process_matrix){
	unsigned char a[20], exp[20];
//	23	17	182	1
//	57	255	3	0
//	7	11	1	1
//	180	200	1	100
//	13	25	101	50

	a[0] = 23;		a[1] = 17;		a[2] = 182;		a[3] = 1;
	a[4] = 57;		a[5] = 255;		a[6] = 3;		a[7] = 0;
	a[8] = 7;		a[9] = 11;		a[10] = 1;		a[11] = 1;
	a[12] = 180;	a[13] = 200;	a[14] = 1;		a[15] = 100;
	a[16] = 13;		a[17] = 25;		a[18] = 101;	a[19] = 50;
	size_t cov = 1;
	MatrixProcessor proc;
	ASSERT_NO_THROW(proc.doAveragingOfElements(a, 4, 5, cov));
	exp[0] = 88;	exp[1] = 89;	exp[2] = 76;	exp[3] = 46;
	exp[4] = 61;	exp[5] = 61;	exp[6] = 52;	exp[7] = 31;
	exp[8] = 118;	exp[9] = 79;	exp[10] = 63;	exp[11] = 17;
	exp[12] = 72;	exp[13] = 59;	exp[14] = 54;	exp[15] = 42;
	exp[16] = 104;	exp[17] = 86;	exp[18] = 79;	exp[19] = 63;
	for (size_t i = 0; i < 20; i++){
		EXPECT_EQ(exp[i], a[i]);
	}
}

TEST(AVG, dont_change_monotone_matrix){
	unsigned char a[20];
	for (size_t i = 0; i < 20; i++){
		a[i] = 100;
	}
	MatrixProcessor proc;
	ASSERT_NO_THROW(proc.doAveragingOfElements(a, 4, 5, 2));

	for (size_t i = 0; i < 20; i++){
		EXPECT_EQ(100, a[i]);
	}
}

TEST(AVG, works_when_cov_is_too_large)
{
	unsigned char a[20];
//	23	17	182	1
//	57	255	3	0
//	7	11	1	1
//	180	200	1	100
//	13	25	101	50

	a[0] = 23;		a[1] = 17;		a[2] = 182;		a[3] = 1;
	a[4] = 57;		a[5] = 255;		a[6] = 3;		a[7] = 0;
	a[8] = 7;		a[9] = 11;		a[10] = 1;		a[11] = 1;
	a[12] = 180;	a[13] = 200;	a[14] = 1;		a[15] = 100;
	a[16] = 13;		a[17] = 25;		a[18] = 101;	a[19] = 50;
	MatrixProcessor proc;
	ASSERT_NO_THROW(proc.doAveragingOfElements(a, 4, 5, 50));
	for (size_t i = 0; i < 20; i++){
		EXPECT_EQ(61, a[i]);
	}
}
