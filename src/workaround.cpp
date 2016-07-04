#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) 
{
	int _width = width;
	int _height = height;
	int _threshold = threshold;
	unsigned char* const _data = data;
	for (int i; i < _width*_height; i++)
	{
		if (_data[i] < _threshold)
			_data[i] = 0;
	}
  // TODO: Add thresholding logic here.
}
