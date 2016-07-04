#pragma once

class MatrixProcessor {
 public:
  void Threshold(unsigned char* const data, const int width, const int height,
                 const int threshold);

  void Average(unsigned char * const data, const int width, const int height, const int range);

  void Median(unsigned char * const data, const int width, const int height, const int range);
};
