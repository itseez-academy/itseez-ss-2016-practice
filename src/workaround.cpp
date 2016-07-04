#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
  // TODO: Add thresholding logic here.
  int n = width * height;
  for (int i = 0; i < n; i++)
    if (data[i] < threshold)
      data[i] = 0;
}

void MatrixProcessor::Average(unsigned char* const data, const int width,
  const int height, const int radius) {
  // TODO: Add thresholding logic here.
  const int n = width * height;
  unsigned char* newdata = new unsigned char[n];
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
    {
      //int index = i * height + j;
      newdata[i * width + j] = GetAverage(data, width, height, i, j, radius);
    }
  for (int i = 0; i < n; i++)
    data[i] = newdata[i];      
}

unsigned char MatrixProcessor::GetAverage(unsigned char* const data, const int width,
  const int height, const int i_pos, const int j_pos, const int radius) {
  int notEmpty = 0;
  unsigned int sum = 0;
  for (int delta = -radius; delta <= radius; delta++)
    for (int i = i_pos - radius; i <= i_pos + radius; i ++)
      for (int j = j_pos - radius; j <= j_pos + radius; j ++)
      {
        if (i < 0 || i >= height || j < 0 || j >= width)
          continue;
        sum += data[i * width + j];
        notEmpty++;
      }
  if (notEmpty == 0) throw "wtf";
  return (unsigned char)(sum / notEmpty);
}
