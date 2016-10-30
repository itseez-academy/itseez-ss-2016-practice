#include "workaround.hpp"

//#include <cstddef>

//using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
  for (int i = 0; i < width * height; i++)
    if (data[i] < threshold)
      data[i] = 0;
}

void MatrixProcessor::Averaging(unsigned char* const data, const int width,
                                const int height, const int surroundings) {
  unsigned char* dst = new unsigned char[width * height];

  for (int i = 0; i < width * height; i++) {
    int sum = 0, count = 0;

    for (int k = -surroundings; k <= surroundings; k++) {
      if (i / width < 1 && k < 0 || i / width == width - 1 && k > 0)
        continue;

      for (int l = -surroundings; l <= surroundings; l++) {
        if (i % width == 0 && l < 0 || (i + 1) % width == 0 && l > 0)
          continue;

        int index = i + k * width + l;

        sum += data[index];
        count++;
      }
    }

    dst[i] = sum / count;
  }

  for (int i = 0; i < width * height; i++)
    data[i] = dst[i];
}

void MatrixProcessor::MedianFilter(unsigned char *const data, const int width,
                                   const int height) {

}
