#pragma once
#include <algorithm>
#include <vector>
class MatrixProcessor {

  int areArgumentsIncorrect(unsigned char* a, int w, int h, int t);
  std::vector<unsigned char> getSubmatrix(unsigned char* const data, const int width, 
  									  const int height, const int cov, const int pos);
  unsigned char getAveragingOfElement(unsigned char* const data, const int width, 
  									  const int height, const int cov, const int pos);
  unsigned char getMedianOfElement(unsigned char* const data, const int width, 
  									  const int height, const int cov, const int pos);
 
 public:
  void Threshold(unsigned char* const data, const int width, const int height,
                 const int threshold);
  void doAveragingOfElements(unsigned char* const data, const int width, 
  							 const int height, const int cov);
  void doMedialProcessing(unsigned char* const data, const int width, 
  							 const int height, const int cov);
};
