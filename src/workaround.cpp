#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) 
{
	int weight = width*height;
	for (int i = 0; i <weight; i++)
	{
		if (data[i] > threshold)
			data[i] = 255;
		else data[i] = 0;
	}
}
