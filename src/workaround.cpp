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
			if (data[j] < threshold)
				data[j] = 0;
		}

	}
}
