#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) 
{
	int weight = width*height;
	for (int i = 0; i <weight; i++)
	{
		if (data[i] > threshold)
			data[i] = 255;
		else data[i] = 0;
	}
}

void MatrixProcessor::Extra_1(unsigned char * const data, const int width, const int height, const int okr)
{
	unsigned char** mass_data = new unsigned char*[width];//  строки в массиве
	for (int i = 0; i < width; i++)
	{
		mass_data[i] = new unsigned char[height]; //  столбцы
	}

	unsigned char** dst = new unsigned char*[width];//  строки в массиве
	for (int i = 0; i < width; i++)
	{
		dst[i] = new unsigned char[height]; //  столбцы
	}

	int k = 0;
	for (int i = 0; i < width; i++)//из одномерного в двумерный
	{
		for (int j = 0; j < height; j++)
		{
			mass_data[i][j] = data[k];
			k++;
		}
	}

	int srz = 0;
	for (int i = 0; i < width; i++)//усредняем
	{
		for (int j = 0; j < height; j++)
		{
			if (i >= okr && j >= okr && width - i >= okr && height - j >= okr)// проверка на окрестность
			{
				for (int k = 0; k < 2 * okr; k++)
				{
					for (int l = 0; l < 2 * okr; l++)
					{
						srz += mass_data[i - okr + k][j - okr + l];
					}

				}
				dst[i][j] = srz / okr / okr;
				srz = 0;
			}
			else
				dst[i][j] = mass_data[i][j];

		}
	}

	int p = 0;
	for (int i = 0; i < width; i++)//из двумерного в одномерный
	{
		for (int j = 0; j < height; j++)
		{
			data[p] = dst[i][j];
			p++;
		}
	}
	for (int i = 0; i<width; i++)
		delete[] mass_data[i];
	delete[] mass_data;

	for (int i = 0; i<width; i++)
		delete[] dst[i];
	delete[] dst;
}
 
