#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (data[i * width + j] > threshold)
			{
				data[i * width + j] = 0;
			}
		}
	}
}

void MatrixProcessor::Average(unsigned char* const data, const int width,
	const int height) {

	int N = width * height;
	unsigned char* src = new unsigned char[N];

	for (int i = 0; i < N; i++)
	{
		src[i] = data[i];
	}

	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			//data[i * width + j] = (src[(height + i - 1) / height * width + (width + j - 1 / width)] + src[(height + i - 1) / height  * width + j] + src[(height + i - 1) / height * width + (j + 1 / width)] +
			//	src[i  * width + (width + j - 1 / width)] + src[i * width + j] + src[i  * width + (j + 1 / width)] +
			//	src[((i + 1) / height)  * width + (width + j - 1 / width)] + src[((i + 1) / height)  * width + j] + src[((i + 1) / height)  * width + (j + 1 / width)]) / 9;

			data[i * width + j] = (src[(i - 1) * width + j - 1] + src[(i - 1)  * width + j] + src[(i - 1) * width + j + 1 ] +
				src[i  * width + j - 1] + src[i * width + j] + src[i  * width + j + 1] +
				src[(i + 1)  * width + j - 1] + src[(i + 1)  * width + j] + src[(i + 1)  * width + j + 1]) / 9;
		}
	}

	delete[] src;
}
