#pragma once

class MatrixProcessor {
 public:
  void Threshold(unsigned char* const data, const int width, const int height,
                 const int threshold);
  void Middle1(unsigned char* const data, const int width, const int height);
  void Middle2(unsigned char* const data, const int width, const int height);
};
