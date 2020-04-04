#ifndef HUFFMAN_DECODER_H_
#define HUFFMAN_DECODER_H_

#include <cstdint>

#include "../BitOperator/BitReader.h"

class HuffmanDecoder {

public:
    /**
     * @brief: Constructor of HuffmanDecoder
     * @param: [buffer] input bit stream
     * @param: [end] the rear pointor of input
     * @param: [sym_bits] the number of bits that every symbols used
     */ 
    HuffmanDecoder(uint8_t *buffer, uint8_t *end, int sym_bits = 8): br_(buffer, end), sym_bits_(sym_bits) { }

    /**
     * @brief: recover lookup table from input bit stream
     */ 
    void readTable();

    /**
     * @brief: recover the compressed content
     * @param: [output] the buffer used to store recoverd result
     * @param: [output_end] the rear pointor of buffer
     */ 
    void decode(uint8_t *output, uint8_t *output_end);

    /**
     * @brief: decode one symbol from bit stream
     * @return: decode result
     */ 
    uint8_t decodeOne();
private:
    /**
     * @brief: build up lookup table
     */ 
    void assignCodes();

    BitReader br_;
    int sym_bits_;
    int num_symbols_;
    int min_codelen_ = 255;
    int max_codelen_ = 0;
    int codelen_count_[17] = {0};

    uint8_t symbol_[256];
    uint8_t bits_to_sym_[0x800]; // 0x800 = 1000 0000 0000, so the index range is 0000 0000 0000 to 0111 1111 1111
    uint8_t bits_to_len_[0x800];

    const int kMaxSymbols__ = 256;

};

#endif