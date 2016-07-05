#pragma once

class MatrixProcessor {
public:
  void Threshold(unsigned char* const data, const int width, const int height,
                 const int threshold);
  void Average(unsigned char* const data, const int width, const int height,
    const int radius);
private:
  unsigned char GetAverage(unsigned char* const data, const int width,
    const int height, const int i_pos, const int j_pos, const int radius);
};
