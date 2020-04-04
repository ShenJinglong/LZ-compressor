#ifndef BIT_WRITER_H_
#define BIT_WRITER_H_

#include <cstdint>
#include <iostream>

#include "../tools/debug.h"

class BitWriter {

public:
    /**
     * @brief: constructor of BitWriter
     * @param: [buffer] the pointer to write
     */ 
    BitWriter(uint8_t *buffer):
        start_(buffer),
        current_(buffer) {

    }

    /**
     * @brief: Write a single bit
     * @param: [v] the bit to write
     */
    void writeBit(int v);

    /**
     * @brief: Writer n bits
     * @param: [v] the bits to write
     * @param: [n] the num of bits to write, should less than 33
     */ 
    void writeBits(int v, int n);

    /**
     * @brief: Flush all bits, and return number of bytes written to buffer
     * @return: the number of bytes written to buffer
     */ 
    int64_t finish();

private:
    /**
     * @brief: Write accumulated bits to buffer
     */ 
    void flush();

    uint8_t *start_;
    uint8_t *current_;
    uint32_t bits_ = 0;
    int position_ = 0;

};

#endif