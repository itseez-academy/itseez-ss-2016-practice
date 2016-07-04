#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
  // TODO: Add thresholding logic here.

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (data[i, j] < threshold)
				data[i, j] = 0;
		}

	}
}
