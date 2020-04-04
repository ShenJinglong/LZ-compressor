#include "utilities.h"

int log2(int v) {
    if (v > 0) {
        return 31 - __builtin_clz(v);
    } else {
        return 0;
    }
}

std::string toBinary(int v, int size) {
    std::string result;
    for (int j = size - 1; j >= 0; --j) {
        result += ((v >> j) & 1) ? "1" : "0";
    }
    return result;
}