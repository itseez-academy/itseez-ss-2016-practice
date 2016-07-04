#pragma once

class MatrixProcessor {
 public:
  void Threshold(unsigned char* const data, const int width, const int height,
                 const int threshold);
  void Median(unsigned char * const data, const int width, const int height, const int range);
};
