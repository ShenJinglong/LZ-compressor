#include "BitReader.h"

int BitReader::readBit() {
    int r = bits_ >> 31;
    bits_ <<= 1;
    ++position_;
    return r;
}

int BitReader::readBits(int n) {
    int r = bits_ >> (32 - n);
    bits_ <<= n;
    position_ += n;
    return r;
}

void BitReader::refill() {
    while (position_ >= 0) {
        bits_ |= (current_ < end_ ? *current_ : 0) << position_;
        position_ -= 8;
        ++current_;
    }
}

void BitReader::byteAlign() {
    int extra_bits = position_ & 7;
    if (extra_bits) {
        readBits(8 - extra_bits);
    }
}