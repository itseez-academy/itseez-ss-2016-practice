#pragma once

class MatrixProcessor {
 public:
  void Threshold(unsigned char* const data, const int width, const int height, const int threshold);
  void Averaging(unsigned char* const data, const int width, const int height, const int location);
  void Median (unsigned char* const data, const int width, const int height, const int location);
  unsigned char * Copy(unsigned char* const data , int size);
  unsigned Max(unsigned char a, unsigned char b);
};
