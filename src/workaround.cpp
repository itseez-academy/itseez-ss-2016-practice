#include "workaround.hpp"

#include <cstddef>
#include <cstdlib>

using namespace std;

int compare(const void * x1, const void * x2)   
{
	return (*(int*)x1 - *(int*)x2);       
}
void MatrixProcessor::Threshold(unsigned char* const _data, const int _width,
                                const int _height, const int _threshold) 
{
	/*int _width = width;
	int _height = height;
	int _threshold = threshold;
	unsigned char* const _data = data;*/
	for (int i = 0; i < _width*_height; i++)
	{
		if (_data[i] < _threshold)
			_data[i] = 0;
	}
  // TODO: Add thresholding logic here.
}
void MatrixProcessor::Middle1(unsigned char* const _data, const int _width, const int _height)
{
	/*int _width = width;
	int _height = height;
	unsigned char* const _data = data;*/
	for (int i = 1; i < _height-1; i++)
	{
		for (int j = 1; j < _width-1; j++)
		{
			_data[i*_width + 1 + j] = (_data[(i - 1)*_width + j] + _data[(i - 1)*_width + 1 + j] + _data[(i - 1)*_width + 2+ j] +
				_data[i*_width + j] + _data[i*_width + 1 + j] + _data[i*_width + 2 + j] +
				_data[(i + 1)*_width + j ] + _data[(i + 1)*_width + 1 + j] + _data[(i + 1)*_width + 2 + j])/9;
		}
	}
	// TODO: Add thresholding logic here.
}
void MatrixProcessor::Middle2(unsigned char* const _data, const int _width, const int _height)
{
	/*int _width = width;
	int _height = height;
	unsigned char* const _data = data;*/
	for (int i = 1; i < _height - 1; i++)
	{
		for (int j = 1; j < _width - 1; j++)
		{
			int *tmp = new int[9];
			tmp[0] = _data[(i - 1)*_width + 1 + j];
			tmp[1] = _data[(i - 1)*_width + 2 + j];
			tmp[2] = _data[(i - 1)*_width + 3 + j];
			tmp[3] = _data[i*_width + 1 + j];
			tmp[4] = _data[i*_width + 2 + j];
			tmp[5] = _data[i*_width + 3 + j];
			tmp[6] = _data[(i + 1)*_width + 1 + j];
			tmp[7] = _data[(i + 1)*_width + 2 + j];
			tmp[8] = _data[(i + 1)*_width + 3 + j];

			qsort(tmp, 9, sizeof(int), compare);     
			_data[i*_width + 1 + j] = tmp[4];
		}
	}
}
