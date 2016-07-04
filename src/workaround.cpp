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
	for (int i = 0; i < _width*_height; i++)
	{
		if (_data[i] < _threshold)
			_data[i] = 0;
	}
  // TODO: Add thresholding logic here.
}
void MatrixProcessor::Middle1(unsigned char* const data, const int width, const int height)
{
	int _width = width;
	int _height = height;
	unsigned char* const _data = data;
	for (int i = 1; i < _height-1; i++)
	{
		for (int j = 1; j < _width-1; j++)
		{
			_data[i*_width + 1 + j] = (_data[(i-1)*_width + j] + _data[(i-1)*_width + 1 + j] + _data[(i-1)*_width + 2 + j] +
				_data[i*_width + j + 1] + _data[i*_width + 2 + j] + _data[i*_width + 3 + j] +
				_data[(i+1)*_width + j + 1] + _data[(i+1)*_width + 2 + j] + _data[(i+1)*_width + 3 + j]) / 9;
		}
	}
	// TODO: Add thresholding logic here.
}
