#pragma once

typedef unsigned char T;

void quickSortR(T* a, long N);

class MatrixProcessor {
 public:
  void Threshold(unsigned char* const data, const int width, const int height,
                 const int threshold);
  void Medium(unsigned char* const data, const int width, const int height);
  void Mediana(unsigned char* const data, const int width, const int height);
private:
	unsigned char MasToPixel(unsigned char* const data, int x, int y, int w, int h, int empty = 0);
	unsigned char MasToPixelS(int x, int y, int empty = 0);
	unsigned char * Mas;
	int W, H;
};
