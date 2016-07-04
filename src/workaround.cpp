#include "workaround.hpp"
#include <iostream>

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (data[i, j] < threshold)
				data[i, j] = 0;
		}

	}
}

void MatrixProcessor::Average(unsigned char* const src, const int width,
	const int height, int locality)
{
	unsigned int* dst = {};
	cout << "average dist";

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{

			dst[i, j] = (src[i - locality, j - locality] + src[i - locality, j] + src[i - locality, j] +
				src[i, j - locality] + src[i, j] + src[i, j] +
				src[i + locality, j - locality] + src[i + locality, j] + src[i + locality, j]) / 9;
		}
	}
}

