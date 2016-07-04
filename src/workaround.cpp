#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
	int length = width*height;
	for (size_t i = 0; i < length; i++)
	{
		if (data[i] < threshold)
			data[i] = 0;
	}
}
