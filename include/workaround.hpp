#pragma once

class MatrixProcessor {
public:
	void Threshold(unsigned char* const data, const int width, const int height,
		const int threshold);
	void AvrgElems(unsigned char* const data, const int width, const int height, const int locality);
	void MedianFilter(unsigned char* const data, const int width, const int height, const int locality);
	unsigned char Sum(unsigned char* const data, const int width, const int height, const int a, const int b, const int locality);
	unsigned char FindAverage(unsigned char* const data, const int width, const int height, const int a, const int b, const int locality);
};
