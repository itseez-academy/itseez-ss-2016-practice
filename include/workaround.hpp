#pragma once

class MatrixProcessor {
 public:
  void Threshold(unsigned char* const data, const int width, const int height,
                 const int threshold);
  void MeanFilter(unsigned char* const data, const int width, const int height);
  void MedianFilter(unsigned char* const data, const int width, const int height);
};
