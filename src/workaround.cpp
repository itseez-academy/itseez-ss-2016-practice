#include "workaround.hpp"

#include <cstddef>

using namespace std;

int MatrixProcessor::areArgumentsIncorrect(unsigned char* a, int w, int h, int t){
	if(a	== 0)	return 1;
	if(w	< 1)	return 1;
	if(h	< 1)	return 1;
	if(t	< 0)	return 1;
	return 0;
}

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {

	if (areArgumentsIncorrect(data, width, height, threshold))
		throw ("Incorrect arguments.");

	for(int i = 0; i < width * height; i++){
		if (data[i] < threshold)
			data[i] = 0;
	}
}

std::vector<unsigned char> MatrixProcessor::getSubmatrix(unsigned char* const data, const int width,
  									  const int height, const int cov, const int pos){

	size_t leftBorder 	= (pos - cov) < 0 ? 0 : (pos - cov);
	size_t upperBorder 	= (pos - cov) < 0 ? 0 : (pos - cov);
	size_t rightBorder 	= (pos + cov) > width ? width : (pos + cov);
	size_t lowerBorder 	= (pos + cov) > height ? height : (pos + cov);
	std::vector<unsigned char> result(	(rightBorder - leftBorder) *
										(lowerBorder - upperBorder));
	size_t count = 0;
	for (size_t i = upperBorder; i <= lowerBorder; i++){
		for (size_t j = leftBorder; j <= rightBorder; j++){
			result[count++] = data[i * width + j];
		}
	}
	return result;
}

unsigned char MatrixProcessor::getAveragingOfElement(unsigned char* const data, const int width, 
													const int height, const int cov, const int pos){
	std::vector<unsigned char> res = getSubmatrix(data, width, height, cov, pos);
	size_t result = 0;
	for(auto x : res){
		result += x;
	}
	return result / res.size();
}

unsigned char MatrixProcessor::getMedianOfElement(unsigned char* const data, const int width, 
													const int height, const int cov, const int pos){
	std::vector<unsigned char> res = getSubmatrix(data, width, height, cov, pos);
	std::sort(res.begin(), res.end());
	return res[res.size() / 2];
}

void MatrixProcessor::doAveragingOfElements(unsigned char* const data, const int width, 
  							 				const int height, const int cov){
	if (areArgumentsIncorrect(data, width, height, cov))
		throw ("Incorrect arguments.");

	const int size = width * height;
	unsigned char* tmpAvg = new unsigned char[size];

	for (size_t i = 0; i < size; i++)
		tmpAvg[i] = getAveragingOfElement(data, width, height, cov, i);
	for (int i = 0; i < width * height; i++)
		data[i] = tmpAvg[i];
	delete[] tmpAvg;
}

void MatrixProcessor::doMedialProcessing(unsigned char* const data, const int width,
  							 const int height, const int cov){
	if (areArgumentsIncorrect(data, width, height, cov))
		throw ("Incorrect arguments.");

	const int size = width * height;
	unsigned char* tmpAvg = new unsigned char[size];

	for (size_t i = 0; i < size; i++)
		tmpAvg[i] = getMedianOfElement(data, width, height, cov, i);
	for (int i = 0; i < width * height; i++)
		data[i] = tmpAvg[i];
	delete[] tmpAvg;
}