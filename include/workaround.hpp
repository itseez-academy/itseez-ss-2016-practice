#pragma once

class MatrixProcessor {
 public:
  void Threshold(unsigned char* const data, const int width, const int height,
                 const int threshold);
  void Extra_1(unsigned char* const data, const int width, const int height,
	  const int okr);

};
