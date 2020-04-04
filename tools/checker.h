#ifndef CHECKER_H_
#define CHECKER_H_

#include <string.h>

#include "debug.h"
#include "utilities.h"
#include "../BitOperator/BitReader.h"
#include "../BitOperator/BitWriter.h"
#include "../HuffmanCodec/HuffmanEncoder.h"
#include "../HuffmanCodec/HuffmanDecoder.h"

/**
 * @brief: check the utilities block
 */ 
void check_utilities() {
    CHECK(log2(16) == 4);
    CHECK(toBinary(15, 8) == "00001111");
    LOGV(0, "UTILITIES CHECKED ...\n");
}

/**
 * @brief: check the bit operator block
 */ 
void check_bit_operator() {
    uint8_t test_buffer[4];
    BitWriter writer(test_buffer);
    writer.writeBits(1, 8);
    writer.writeBits(2, 16);
    writer.writeBits(3, 7);
    writer.writeBit(1);
    writer.finish();
    BitReader reader(test_buffer, test_buffer + 4);
    CHECK(reader.readBits(8) == 1);
    CHECK(reader.readBits(16) == 2);
    CHECK(reader.readBits(7) == 3);
    CHECK(reader.readBit() == 1);
    LOGV(0, "BIT OPERATOR CHECKED ...\n");
}

/**
 * @brief: check huffman codec block
 */ 
void check_huffman_codec() {
    int64_t len = 20;
    uint8_t buffer_in[len] = {5, 5, 5, 5, 6, 6, 6, 7, 7, 8, 9, 4, 5, 2, 1, 7, 4, 5, 3, 8};
    uint8_t buffer_out[len + 385]; // the maximum size of huffman tree table is 385 bytes (256 + 256 / 2 + 1)
    uint8_t buffer_recover[len];
    
    HuffmanEncoder encoder(buffer_out);
    for (int64_t i = 0; i < len; ++i) {
        encoder.scan(buffer_in[i]);
    }
    encoder.buildTable();
    for (int64_t i = 0; i < len; ++i) {
        encoder.encode(buffer_in[i]);
    }
    encoder.finish(); // Don't forget to call finish(), this function is used to make sure that all bits in the buffer are written to bit stream.
    
    // BitReader reader(buffer_out, buffer_out + len + 385);
    // for (int i = 0; i < len + 385; ++i) {
    //     reader.refill();
    //     LOGV(2, std::string(toBinary(reader.readBits(8), 8) + "\n").c_str());
    // }
    
    HuffmanDecoder decoder(buffer_out, buffer_out + len + 385);
    decoder.readTable();
    decoder.decode(buffer_recover, buffer_recover + len);
    // for (int i = 0; i < len; ++i) {
    //     LOGV(1, "%d\n", buffer_recover[i]);
    // }
    CHECK(memcmp(buffer_in, buffer_recover, len) == 0);

    LOGV(0, "HUFFMAN CODEC CHECKED ...\n");
}

/**
 * @brief: check the whole system
 */ 
void check_all() {
    check_utilities();
    check_bit_operator();
    check_huffman_codec();
    LOGV(0, "ALL BLOCKS HAVE CHECKED ...\n");
}

#endif