#ifndef BIT_READER_H_
#define BIT_READER_H_

#include <cstdint>

class BitReader {

public:
    /**
     * @brief: Constructor of BitReader
     * @param: [buffer] The head pointer of file
     * @param: [end] The trail pointer of file
     */
    BitReader(uint8_t *buffer, uint8_t *end):
        current_(buffer),
        end_(end) {
        refill();
    }

    /**
     * @brief: Consume one bit, Result is 0 or 1
     * @return: The top bit, 0 or 1
     */
    int readBit();

    /**
     * @brief: Read n bits from the buffer
     * @param: [n] the num of bits that you want to read. should less than 33
     * @return: the data read from buffer
     */ 
    int readBits(int n);

    /**
     * @brief: Fill buffer with up to 32 bits
     */
    void refill();

    /**
     * @brief: align the bytes in the buffer
     */ 
    void byteAlign();

    /**
     * @brief: get the rear pointer of input bit stream
     * @return: the pointer
     */ 
    uint8_t *end() const {
        return end_;
    }

    /**
     * @brief: get the buffer
     * @return: the buffer in this object
     */ 
    uint32_t bits() const {
        return bits_;
    }

    /**
     * @brief: Return actual location we have read up to in the byte stream.
     * @return: The location
     */ 
    uint8_t *cursor() const {
        return current_ - ((24 - position_) / 8);
    }

private:
    uint8_t *current_;
    uint8_t *end_;
    uint32_t bits_ = 0;
    int position_ = 24;
};

#endif