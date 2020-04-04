#ifndef HUFFMAN_ENCODER_H_
#define HUFFMAN_ENCODER_H_

#include <algorithm>

#include "../BitOperator/BitWriter.h"
#include "../tools/debug.h"
#include "../tools/utilities.h"

class HuffmanEncoder{

public:
    /**
     * @brief: Constructor of HuffmanEncoder
     * @param: [buffer] the buffer for output
     * @param: [max_symbols = 256] the maximum amount of symbols
     */ 
    HuffmanEncoder(uint8_t *buffer, int max_symbols = 256):
        writer_(buffer), max_symbols_(max_symbols) {
        for (int i = 0; i < max_symbols_; ++i) {
            nodes_[i].symbol = i;
            nodes_[i].freq = 0;
        }
    }

    /**
     * @brief: Count each symbol
     * @param: [symbol] the symbol which to be counted
     */ 
    void scan(int symbol) {
        ++nodes_[symbol].freq;
    }

    /**
     * @brief: build huffman tree table and write it to output bit stream
     */ 
    void buildTable();

    /**
     * @brief: encode specific symbol, and write the result to output bit stream
     * @param: [symbol] the symbol to be coded
     */ 
    void encode(int symbol) {
        writer_.writeBits(code_[symbol], length_[symbol]);
    }

    /**
     * @brief: flush all the bits to output bit stream
     * @return: the total number of bytes that have been written to bit stream
     */ 
    int64_t finish() {
        return writer_.finish();
    }

private:
    /**
     * @brief: this data structure is used to build up buffman tree, represent a node
     */ 
    struct Node {
        int freq;
        int symbol;
        Node *l;
        Node *r;
    };

    /**
     * @brief: class function, used to build up small root heap
     */ 
    struct Comparator {
        bool operator()(const Node *l, const Node *r) {
            return l->freq > r->freq;
        }
    };
    
    /**
     * @brief: determine code length (the deepth of node) of every symbol
     * @param: [n] root node
     * @param: [level] the deepth of root node, generally it equals 0, but if we have only one symbol, then it equals 1 (for you have to use at least one bit to code the symbol)
     */ 
    void walk(Node *n, int level);

    /**
     * @brief: used to build up lookup table
     * @param: [num_symbols] the num of symbols that we need to code
     */
    void buildCodes(int num_symbols);

    /**
     * @brief: write huffman tree table to output bit stream
     * @param: [num_symbols] the num of symbols that we need to code
     */ 
    void writeTable(int num_symbols);

    /**
     * @brief: limit huffman code length of each symbol (based on kraft inequation)
     * @param: [num_symbols] the num of symbols that we need to code
     */ 
    void limitLength(int num_symbols);

    BitWriter writer_;
    int max_symbols_;
    
    Node nodes_[512];
    uint8_t length_[256];
    int code_[256];

    const int kMaxHuffCodeLength__ = 11;

};

#endif