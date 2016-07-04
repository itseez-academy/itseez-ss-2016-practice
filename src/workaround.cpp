#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
	for (int i = 0; i < width * height; i++)
	{
		char tmp = data[i];
		data[i] = tmp < threshold ? 0 : tmp;
	}
}


void MatrixProcessor::Median(unsigned char * const data, const int width, const int height, const int range)
{
	int maskWidth = range * 2 + 1;
	int maskSize = maskWidth * maskWidth;

	int minX = range, minY = range;
	int maxX = width - range, maxY = height - range; //it's more than max, actually

	if (height >= maskWidth && width >= maskWidth)
	{
		int changedSubmatrixWidth = maxX - minX;
		int changedSubmatrixHeight = maxY - minY;
		unsigned char * newData = new unsigned char[changedSubmatrixWidth * changedSubmatrixHeight];

		for (int i = minX; i < maxX; i++)
			for (int j = minY; j < maxY; j++)
			{

				int sum = 0;
				for (int ii = i - range; ii <= i + range; ii++)
					for (int jj = j - range; jj <= j + range; jj++)
						sum += data[ii + jj * width];

				newData[i - minX + (j - minY) * changedSubmatrixWidth] = sum / maskSize;
			}

		for (int i = minX; i < maxX; i++)
			for (int j = minY; j < maxY; j++)
			{
				data[i + j * width] = newData[i - minX + (j - minY) * changedSubmatrixWidth];
			}
	}
}