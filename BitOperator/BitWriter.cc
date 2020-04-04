#include "BitWriter.h"

void BitWriter::writeBit(int v) {
    bits_ = (bits_ << 1) | v;
    ++position_;
    if (position_ >= 8) {
        flush();
    }
}

void BitWriter::writeBits(int v, int n) {
    bits_ = (bits_ << n) | v;
    position_ += n;
    if (position_ >= 8) {
        flush();
    }
}

int64_t BitWriter::finish() {
    flush();
    CHECK(position_ >= 0 && position_ < 8);
    if (position_ > 0) {
        *current_ = (bits_ & ((1 << position_) - 1)) << (8 - position_);
        ++current_;
        position_ = 0;
    }
    return current_ - start_;
}

void BitWriter::flush() {
    while (position_ >= 8) {
        position_ -= 8;
        *current_ = (bits_ >> position_) & 0xFF;
        ++current_;
    }
}

