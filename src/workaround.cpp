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
	for(int i= 0; i < height; i++ )
		for (int j = 0; i < width; i++)
		{
			// угловые элементы
			if ((i == 0) && (j == 0))
				data[i][j] = (data[i, j] + data[i + 1, j] + data[i + 1, j + 1] + data[i, j + 1]) / 4;
			if ((i == 0) && (j == width-1))
				data[i, j] = (data[i, j] + data[i - 1, j] + data[i - 1, j + 1] + data[i, j + 1]) / 4;
			if ((i == height - 1) && (j == 0))
				data[i, j] = (data[i, j] + data[i - 1, j] + data[i + 1, j - 1] + data[i, j - 1]) / 4;
			if ((i == height - 1) && (j == width- 1))
				data[i, j] = (data[i, j] + data[i -1, j -1] + data[i, j - 1] + data[i-1, j]) / 4;
			//окаймл€ющие элементы
			if ((i == 0) && (j > 0) && (j < width - 1))
				data[i, j] = (data[i, j] + data[i + 1, j] + data[i + 1, j + 1] + data[i, j + 1] + data[i, j - 1] + data[i + 1, j - 1]) / 6;
			if (j == 0 && i > 0 && i < height - 1)
				data[i, j] = (data[i, j] + data[i + 1, j] + data[i + 1, j + 1] + data[i, j + 1] + data[i - 1, j] + data[i - 1, j + 1]) / 6;
			if (i == height -1  && j > 0 && j < width - 1)
				data[i, j] = (data[i, j] + data[i -1 , j] + data[i - 1, j + 1] + data[i, j + 1] + data[i - 1, j-1] + data[i, j - 1]) / 6;
			if (j == width -1  && i > 0 && i < height - 1)
				data[i, j] = (data[i, j] + data[i + 1, j] + data[i + 1, j -1 ] + data[i, j - 1] + data[i - 1, j -1 ] + data[i - 1, j]) / 6;

			
			//центральные элементы
			
			if (i < width - 1 && i && j > 0 && j < height - 1)
				data[i, j] = (data[i - 1, j - 1] + data[i - 1, j] + data[i - 1, j + 1] +
					data[i, j - 1] + data[i, j] + data[i, j + 1] +
					data[i + 1, j - 1] + data[i + 1, j] + data[i + 1, j + 1]) / 9;

			
		}
}
