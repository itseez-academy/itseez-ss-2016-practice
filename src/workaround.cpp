#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
	for (int i = 0; i < width*height; i++)
		if (data[i] < threshold)
			data[i] = 0;
}

void MatrixProcessor::Medium(unsigned char* const data, const int width, const int height)
{
	Mas = data;
	W = width;
	H = height;
	int i = 0;
	for (int y = 0; y < width; y++)
		for (int x = 0; x < height; y++)
		{
			data[i] = (MasToPixelS(x - 1, y - 1, data[i]) + MasToPixelS(x - 1, y, data[i]) + MasToPixelS(x - 1, y, data[i]) + MasToPixelS(x, y - 1, data[i]) +
				MasToPixelS(x, y, data[i]) + MasToPixelS(x, y, data[i]) + MasToPixelS(x + 1, y - 1, data[i]) + MasToPixelS(x + 1, y, data[i]) +
				MasToPixelS(x + 1, y, data[i])) / 9;
			i++;
		}
}

void MatrixProcessor::Mediana(unsigned char* const data, const int width, const int height)
{
	Mas = data;
	W = width;
	H = height;
	int i = 0;
	for (int y = 0; y < width; y++)
		for (int x = 0; x < height; y++)
		{
			data[i] = (MasToPixelS(x - 1, y - 1, data[i]) + MasToPixelS(x - 1, y, data[i]) + MasToPixelS(x - 1, y + 1, data[i])
				+ MasToPixelS(x, y - 1, data[i]) +	MasToPixelS(x, y, data[i]) + MasToPixelS(x, y + 1, data[i]) 
				+ MasToPixelS(x + 1, y - 1, data[i]) + MasToPixelS(x + 1, y, data[i]) +	MasToPixelS(x + 1, y + 1, data[i])) / 9;
			i++;
		}
}

unsigned char MatrixProcessor::MasToPixelS(int x, int y, int empty = 0)
{
	MasToPixel(Mas, x, y, W, H, empty);
}



unsigned char MatrixProcessor::MasToPixel(unsigned char* const data, int x, int y, int w, int h, int empty = 0)
{
	if (x > 0 && x < w&&y>0 && x < h)
		return data[y*w + x];
	else return empty;
}