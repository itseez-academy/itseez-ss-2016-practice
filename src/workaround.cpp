#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
  // TODO: Add thresholding logic here.
	int i = 0;
	int j = 0;
	while (i < height)
	{
		
		while (j < width)
		{
			
			if (data[i * width + j] < threshold)
				data[i * width + j] = 0;
			j += 1;

		}

		j = 0;
		i += 1;
	}
}
