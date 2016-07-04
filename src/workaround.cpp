#include "workaround.hpp"

#include <cstddef>
#include<vector>
#include<numeric>
using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
  // TODO: Add thresholding logic here.
	for (int i = 0; i < width*height; i++) {
		if (data[i] < threshold)data[i] = 0;
	}
}

void MatrixProcessor::MeanFilter(unsigned char* const data, const int width, const int height) 
{
	vector<unsigned char> data_copy(data, data + (width*height));
	for (int i = 1; i <width-1; i++)
	{
		
		for (int j = 1; j < height-1; j++)
		{
			data[i*width+j] =(
				data_copy[(i - 1)*width + (j - 1)] +
				data_copy[(i )*width + (j - 1)] +
				data_copy[(i +1)*width + (j - 1)] +
				data_copy[(i - 1)*width + (j )] +
				data_copy[(i )*width + (j )] +
				data_copy[(i + 1)*width + (j )] +
				data_copy[(i - 1)*width + (j + 1)] +
				data_copy[(i )*width + (j + 1)] +
				data_copy[(i + 1)*width + (j + 1)])/9;
		}
	}
	
}