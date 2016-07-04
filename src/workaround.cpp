#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
  // TODO: Add thresholding logic here.
	int length = width*height;
	for (int i = width+1; i < length-641; i++)
	{
		data[i] = (data[i - width+1] + data[i - width] + data[i - width-1] + data[i-1] + data[i] + data[i+1] + data[i + width-1] + data[i + width] + data[i + width+1]) / 9;
	}
}
