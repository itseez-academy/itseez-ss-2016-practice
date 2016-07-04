#pragma once

class MatrixProcessor {
 public:
  void Threshold(unsigned char* const data, const int width, const int height,
                 const int threshold);
  void Averaging(unsigned char* const data, const int width, const int height,
				 const int surroundings);
  void Median(unsigned char* const data, const int width, const int height,
				 const int surroundings);
};
