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
