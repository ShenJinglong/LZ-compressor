#include <algorithm>
#include <string.h>

#include "HuffmanDecoder.h"
#include "../tools/debug.h"
#include "../tools/utilities.h"

void HuffmanDecoder::readTable() {
    br_.refill();
    num_symbols_ = br_.readBits(sym_bits_); // in the range of 0 to 255

    CHECK(num_symbols_ < kMaxSymbols__);

    for (int i = 0; i <= num_symbols_; ++i) {
        br_.refill();
        int symbol = br_.readBits(sym_bits_);
        int codelen = br_.readBits(4) + 1;
        LOGV(10, "sym:%d len:%d\n", symbol, codelen);

        ++codelen_count_[codelen];
        symbol_[i] = symbol;
        min_codelen_ = std::min(min_codelen_, codelen);
        max_codelen_ = std::max(max_codelen_, codelen);
    }
    LOGV(10, "num_sym %d codelen(min:%d, max:%d)\n", num_symbols_ + 1, min_codelen_, max_codelen_);

    // Ensure we catch up to be aligned.
    br_.byteAlign();

    assignCodes();
}

void HuffmanDecoder::decode(uint8_t *output, uint8_t *output_end) {
    uint8_t *src = br_.cursor();
    uint8_t *src_end = br_.end();
    int position = 24;
    uint32_t bits = 0;

    for (;;) {
        while (position >= 0) {
            bits |= (src < src_end ? *src++ : 0) << position;
            position -= 8;
        }
        int n = bits >> (32 - max_codelen_);
        int len = bits_to_len_[n];
        *output++ = bits_to_sym_[n];
        if (output >= output_end) {
            break;
        }
        bits <<= len;
        position += len;
    }
}

uint8_t HuffmanDecoder::decodeOne() {
    br_.refill();
    int n = br_.bits() >> (32 - max_codelen_);
    int len = bits_to_len_[n];
    br_.readBits(len);
    return bits_to_sym_[n];
}

void HuffmanDecoder::assignCodes() {
    int p = 0;
    uint8_t *cursym = &symbol_[0];
    for (int i = min_codelen_; i <= max_codelen_; ++i) {
        int n = codelen_count_[i];
        if (n) {
            int shift = max_codelen_ - i;
            memset(bits_to_len_ + p, i, n << shift);
            int m = 1 << shift;
            do {
                memset(bits_to_sym_ + p, *cursym++, m);
                p += m;
            } while (--n);
        }
    }
}