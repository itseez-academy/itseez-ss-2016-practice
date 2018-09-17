#pragma once

class MatrixProcessor {
 public:
  void Threshold(unsigned char* const data, const int width, const int height,
                 const int threshold);
  void AvrgElems(unsigned char* const data, const int width, const int height, const int locality);
  
};
