#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
  // TODO: Add thresholding logic here.

	for (int i = 0; i < width * height; ++i)
	{
		if (data[i] < threshold)
			data[i] = 0;
	}
}

void MatrixProcessor::Averaging(unsigned char * const data, const int width, const int height)
{
	for (int j = 1; j < width - 1; ++j)
		for (int i = 1; i < height - 1; ++i)
			data[i*width + j] = (data[(i-1)*width + j -1] + data[(i-1)*width + j] + data[(i-1)*width + j + 1] + 
			                    data[i*width + j - 1] + data[i*width + j] + data[i*width + j + 1] +
			                    data[(i+1)*width + j - 1] + data[(i+1)*width + j]  + data[(i+1)*width + j + 1])/9;
}

