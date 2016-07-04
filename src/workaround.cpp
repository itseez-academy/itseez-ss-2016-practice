#include "workaround.hpp"
#include <algorithm>
#include <cstddef>
#include<vector>
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
	for (int i = 1; i < height-1; i++)
	{
		
		for (int j = 1; j < width-1; j++)
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

void MatrixProcessor::MedianFilter(unsigned char* const data, const int width, const int height)
{
	vector<unsigned char> data_copy(data, data + (width*height));
	for (int i = 1; i < height - 1; i++)
	{

		for (int j = 1; j < width - 1; j++)
		{
			unsigned char roi[9];
			roi[0]=(data_copy[(i - 1)*width + (j - 1)]);
			roi[1]=(data_copy[(i)*width + (j - 1)]);
			roi[2]=(data_copy[(i + 1)*width + (j - 1)] );
			roi[3]=(data_copy[(i - 1)*width + (j)] );
			roi[4]=(data_copy[(i)*width + (j)] );
			roi[5]=(data_copy[(i + 1)*width + (j)] );
			roi[6]=(data_copy[(i - 1)*width + (j + 1)] );
			roi[7]=(data_copy[(i)*width + (j + 1)] );
			roi[8]=(data_copy[(i + 1)*width + (j + 1)]);
			sort(roi, roi+9);
			data[i*width+j]=roi[4];
		}
	}
}
