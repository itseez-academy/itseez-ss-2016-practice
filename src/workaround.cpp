#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
	// TODO: Add thresholding logic here.
	for (int i = 0; i < width*height; i++) {
		data[i] = data[i] > threshold ? 255 : 0;
	}
}
