#include "workaround.hpp"

#include <cstddef>
#include <stdexcept>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
    if (data != nullptr) {
        for (int i = 0; i < width * height; i++)
            if (data[i] < threshold)
                    data[i] = 0;
    } else {
        throw std::invalid_argument("Data is empty!");
    }
}
