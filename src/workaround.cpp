#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
	for (int i = 0; i <height*width; i++)

		if (data[i] < threshold) data[i] = 0;
		else data[i] = 255;


		
		// TODO: Add thresholding logic here.
}

/*
void MatrixProcessor::window(unsigned char* const data,unsigned char* const dest,
	const int width,
	const int height, const int x0, const int y0)
	
{
	
	for (int k = 0; k < width * height - 1; k++)
	{
		dest[i*width + j] = data[(x0 + i)*width + y0 + j];
	}


}
	

*/
