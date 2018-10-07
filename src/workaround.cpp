#include "workaround.hpp"

#include <cstddef>
#include <algorithm>
using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) 
{
	for (int i = 0; i < width*height; i++)
		if (data[i] < threshold)
			data[i] = 0;
}

void MatrixProcessor::AvrgElems(unsigned char* const data, const int width, 
	                                        const int height, const int locality)
{
	int ind;
	unsigned char* tmpData = new unsigned char[width*height];
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{	
			ind = i * width + j;
			tmpData[ind] = Sum(data, width, height, i, j, locality);
		}

	for (int i = 0; i < width*height; i++)
		data[i] = tmpData[i];
}

void MatrixProcessor::MedianFilter(unsigned char * const data, const int width, 
	                                             const int height, const int locality)
{
	unsigned char* tmpData = new unsigned char[width*height];
	int ind = 0;

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			ind = i * width + j;
			tmpData[ind] = FindAverage(data, width, height, i, j, locality);
		}

	for (int i = 0; i < width*height; i++)
		data[i] = tmpData[i];
}

unsigned char MatrixProcessor::Sum(unsigned char * const data, const int width, 
	                                  const int height, const int a, const int b, const int locality)
{
	int m = 0;
	int sum = 0;

	for (int i = a - locality; i <= a + locality; i++)
		for (int j = b - locality; j <= b + locality; j++)
		{
			if (i < 0 || i >= height || j < 0 || j >= width)
				continue;
			sum += data[i * width + j];
			m++;
		}

	if (m == 0) throw "1";

	return (unsigned char)(sum / m);
}

unsigned char MatrixProcessor::FindAverage(unsigned char * const data, const int width, const int height,
	                                                            const int a, const int b, const int locality)
{
	unsigned char* avrg = new unsigned char[(locality * 2 + 1)*(locality * 2 + 1)];
	int ind = 0;

	for (int i = a - locality; i <= a + locality; i++)
		for (int j = b - locality; j <= b + locality; j++)
		{
			if (i < 0 || i >= height || j < 0 || j >= width)
			{
				continue;
			}
			avrg[ind++] = data[i * width + j];
		}

	std::sort(avrg, avrg + ind + 1);

	return avrg[(ind + 1) / 2];
}
