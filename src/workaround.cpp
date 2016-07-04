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
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
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
	unsigned char mas[9];
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			mas[0] = MasToPixelS(x - 1, y - 1, data[i]);
			mas[1] = MasToPixelS(x - 1, y, data[i]);

			mas[2] = MasToPixelS(x - 1, y + 1, data[i]);
				mas[3] = MasToPixelS(x, y - 1, data[i]);
				mas[4] = MasToPixelS(x, y, data[i]);
				mas[5] = MasToPixelS(x, y + 1, data[i]);
				mas[6] = MasToPixelS(x + 1, y - 1, data[i]);
				mas[7] = MasToPixelS(x + 1, y, data[i]);
				mas[8] = MasToPixelS(x + 1, y + 1, data[i]);
				quickSortR(mas, 9);
				data[i] = mas[4];
				i++;
		}
}

unsigned char MatrixProcessor::MasToPixelS(int x, int y, int empty)
{
	return MasToPixel(Mas, x, y, W, H, empty);
}



unsigned char MatrixProcessor::MasToPixel(unsigned char* const data, int x, int y, int w, int h, int empty)
{
	if (x > 0 && x < w&&y>0 && x < h)
		return data[y*w + x];
	else return empty;
}


void quickSortR(T* a, long N) {
	// На входе - массив a[], a[N] - его последний элемент.

	long i = 0, j = N - 1; 		// поставить указатели на исходные места
	T temp, p;

	p = a[N >> 1];		// центральный элемент

						// процедура разделения
	do {
		while (a[i] < p) i++;
		while (a[j] > p) j--;

		if (i <= j) {
			temp = a[i]; a[i] = a[j]; a[j] = temp;
			i++; j--;
		}
	} while (i <= j);


	// рекурсивные вызовы, если есть, что сортировать 
	if (j > 0) quickSortR(a, j);
	if (N > i) quickSortR(a + i, N - i);
}