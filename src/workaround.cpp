#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
	for (int i = 0; i < width * height; i++)
		if (data[i] < threshold) 
			data[i] = 0;
}

void MatrixProcessor::Averaging(unsigned char* const data, const int width,
								const int height, const int surroundings) {
	int maxSize = width * height;
	for (int i = 0; i < width * height; i++) {
		int sum = 0;
		int count = 0;
		for (int j = -surroundings; j < surroundings + 1; j++)
			for (int k = -surroundings; k < surroundings + 1; k++) {
				int idx = i + j*width + k;
				if ((idx < 0) || (idx > maxSize))
					continue;
				sum += data[i + j*width + k];
				count++;
			}
		data[i] = sum / count;

	}
}
