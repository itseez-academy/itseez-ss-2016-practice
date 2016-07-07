#include "workaround.hpp"

#include <cstddef>
#include<array>
#include<vector>
using namespace std;
unsigned MatrixProcessor::Max(unsigned char a, unsigned char b) {
	if (a > b) return 0;
	if (a < 0) return 0;
}
unsigned char * MatrixProcessor::Copy(unsigned char* const data, int size) {
	unsigned char *tmp = new unsigned char[size];
	for (size_t i = 0; i < size; i++)
		tmp[i] = data[i];
	return tmp;
};
void MatrixProcessor::Threshold(unsigned char* const data, const int width, const int height, const int threshold) {
	int length = width*height;
	for (size_t i = 0; i < length; i++)
	{
		if (data[i] < threshold)
			data[i] = 0;
	}
}
void MatrixProcessor::Averaging(unsigned char* const data, const int width, const int height, const int location) {
	int length = width*height,tmp;
	unsigned char *src = Copy(data, length);
	
	
	for (size_t k = 0; k < length; k++)
	{
		for (size_t i = - location /2; i <= location/2; i++)
		{
			for (size_t j = -location / 2; j <= location / 2; j++)
				tmp += src[Max(k + j + i*width,width)] ;
		}
		data[k] = tmp / location*location;
	}


	delete src;
}
void MatrixProcessor::Median(unsigned char* const data, const int width, const int height, const int location) {
	
	int length = width*height, tmp;
	unsigned char *src = Copy(data, length);
	
	
	for (size_t k = 0; k < length; k++)
	{
		//for (size_t i = -location / 2; i <= location / 2; i++)
		//{
		//	for (size_t j = -location / 2; j <= location / 2; j++)
		//	// v.push_back(src[Max(k + j + i*width, width)]);
		//}
	;
	}

	delete src;
}
