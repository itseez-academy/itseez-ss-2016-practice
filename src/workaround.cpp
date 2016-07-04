#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {

	if(data == NULL)
		throw("Incorrect data");
	if(width < 1)
		throw("Incorrect width");
	if(height < 1)
		throw("Incorrect height");
	if(threshold < 0)
		throw("Incorrect threshold");

	for(int i = 0; i < width * height; i++){
		if (data[i] < threshold)
			data[i] = 0;
	}
}
