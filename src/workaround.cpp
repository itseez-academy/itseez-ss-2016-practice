#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
  for(int i = 0; i < width * height; i++){
    if (data[i] < threshold)
      data[i] = 0;
  }
}
