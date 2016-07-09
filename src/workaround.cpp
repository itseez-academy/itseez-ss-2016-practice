#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
  // TODO: Add thresholding logic here.
	for (int i = 0; i < width*height; i++)
	{
		if (data[i] < threshold)
			data[i] = 0;
	}
}
void MatrixProcessor::Mean(unsigned char* const data, const int width,const int height) 
{
	for (int i = width + 1; i < width*height - width - 2; i++)
	{
		if ((i%width!= 0) && ( (i + 1)%width != 0))
			data[i] = (data[i - width - 1] + data[i - width] + data[i - width + 1] +
				data[i - 1] + data[i] + data[i + 1] +
				data[i + width - 1] + data[i + width] + data[i + width + 1]) / 9;
	}
	
}
