#include "workaround.hpp"

#include <cstddef>

using namespace std;

//- Проверка корректности аргументов
int MatrixProcessor::areArgumentsIncorrect(unsigned char* a, int w, int h, int t){
	if(a	== 0)	return 1;
	if(w	< 1)	return 1;
	if(h	< 1)	return 1;
	if(t	< 0)	return 1;
	return 0;
}

//+ Пороговая обработка
void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {

	if (areArgumentsIncorrect(data, width, height, threshold))
		throw ("Incorrect arguments.");

	for(int i = 0; i < width * height; i++){
		if (data[i] < threshold)
			data[i] = 0;
	}
}

//- Получение вектора подматрицы окрестности элемента
std::vector<unsigned char> MatrixProcessor::getSubmatrix(unsigned char* const data, const int width,
  									  const int height, const int cov, const int pos){
	const int	line	= pos / width,
				column	= pos % width;

	const int	left	= max(0, column - cov),
				right	= min(width - 1, column + cov),
				upper	= max(0, line - cov),
				lower	= min(height - 1, line + cov);

	//====SUBMATRIX===
	//			upper
	//  |-------------------|
	//	|		 || 		|
	// l|	    (cov)		|r
	// e|		 || 		|i
	// f| <cov>	 pos  <cov> |g
	// t|		 || 		|h
	//  |	    (cov)		|t
	//	|		 || 		| 
	//	|-------------------|
	//			lower

	std::vector<unsigned char> result((right - left + 1) * (lower - upper + 1));
	size_t count = 0;
	for (int i = upper; i <= lower; i++) {
		for (int j = left; j <= right; j++) {
			result[count++] = data[i * width + j];
		}
	}
	return result;
}

//- Усреднение конкретного элемента
unsigned char MatrixProcessor::getAveragingOfElement(unsigned char* const data, const int width, 
								const int height, const int cov, const int pos){
	std::vector<unsigned char> res = getSubmatrix(data, width, height, cov, pos);
	int result = 0;
	for_each(res.begin(), res.end(), [&](unsigned char x) {	result = result + x;});
	return result / res.size();
}

//- Получение медианы конкретного элемента
unsigned char MatrixProcessor::getMedianOfElement(unsigned char* const data, const int width, 
								const int height, const int cov, const int pos){
	std::vector<unsigned char> res = getSubmatrix(data, width, height, cov, pos);
	std::sort(res.begin(), res.end());
	return res[res.size() / 2];
}

//+ Усреднение матрицы
void MatrixProcessor::doAveragingOfElements(unsigned char* const data, const int width, 
  							 				const int height, const int cov){
	if (areArgumentsIncorrect(data, width, height, cov))
		throw ("Incorrect arguments.");

	int size = width * height;
	unsigned char* tmpAvg = new unsigned char[size];

	for (size_t i = 0; i < size; i++) {
		tmpAvg[i] = getAveragingOfElement(data, width, height, cov, i);
	}

	for (int i = 0; i < size; i++)
		data[i] = tmpAvg[i];
	delete[] tmpAvg;
}

//+ Медиальная обработка
void MatrixProcessor::doMedialProcessing(unsigned char* const data, const int width,
  							 const int height, const int cov){
	if (areArgumentsIncorrect(data, width, height, cov))
		throw ("Incorrect arguments.");

	const int size = width * height;
	unsigned char* tmpAvg = new unsigned char[size];

	for (size_t i = 0; i < size; i++)
		tmpAvg[i] = getMedianOfElement(data, width, height, cov, i);
	for (int i = 0; i < width * height; i++)
		data[i]	= tmpAvg[i];
	delete[] tmpAvg;
}
