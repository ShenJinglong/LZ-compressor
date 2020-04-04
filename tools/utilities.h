#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <chrono>
#include <string>

struct Timer {
    Timer(): time_(std::chrono::high_resolution_clock::now()) { }

    double elapsed() const {
        return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - time_).count();
    }

    std::chrono::high_resolution_clock::time_point time_;
};

/**
 * @brief: fast log2 function
 * @param: [v] input number
 * @return: the result after log2
 */ 
int log2(int v);

/**
 * @brief: transform the inputed integer to binary string
 * @param: [v] input number
 * @param: [size] the size of binary string
 * @return: the result after transform
 */
std::string toBinary(int v, int size);


#endif