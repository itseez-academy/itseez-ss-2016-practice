#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
  // TODO: Add thresholding logic here.
	for (int i = 0; i < width*height; i++)
		if (data[i] < threshold)
			data[i] = 0;
	

}
void MatrixProcessor::AvrgElems(unsigned char* const data, const int width, const int height, const int locality)
{
	int a = 0;
	int ind;
	int l = (2 * locality + 1)*(2 * locality + 1);
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

int MatrixProcessor::Sum(unsigned char * const data, const int width, const int height, const int a, const int b, const int locality)
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
