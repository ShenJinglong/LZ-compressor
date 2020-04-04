#include "HuffmanEncoder.h"

void HuffmanEncoder::buildTable() {
    // Coalesce used symbols, and add to heap
    
    Node *q[256];
    int num_symbols = 0;
    for (int i = 0; i < max_symbols_; ++i) {
        if (nodes_[i].freq) {
            nodes_[num_symbols] = nodes_[i];
            q[num_symbols] = &nodes_[num_symbols];
            ++num_symbols;
        }
    }

    Comparator c;
    std::make_heap(&q[0], &q[num_symbols], c);
    // Build Huffman tree
    for (int i = num_symbols; i > 1; --i) {
        Node *n1 = q[0];
        std::pop_heap(&q[0], &q[i], c);
        Node *n2 = q[0];
        std::pop_heap(&q[0], &q[i - 1], c);

        Node *parent = &nodes_[num_symbols + i - 2];
        parent->freq = n1->freq + n2->freq;
        parent->symbol = -1;
        parent->l = n2;
        parent->r = n1;
        q[i - 2] = parent;
        std::push_heap(&q[0], &q[i - 1], c);
    }

    // Label the distances from the root for the leafs
    walk(q[0], num_symbols == 1 ? 1 : 0);

    // Sort leaf nodes into level order, This is required
    // for both length limiting and writing the table
    std::sort(&nodes_[0], &nodes_[num_symbols], [](const Node &l, const Node &r) { return l.freq < r.freq; });
    
    limitLength(num_symbols);
    writeTable(num_symbols);
    buildCodes(num_symbols);
}

void HuffmanEncoder::buildCodes(int num_symbols) {
    int code = 0;
    int last_level = -1;
    LOGV(10, "Write num_symbols %d\n", num_symbols);
    for (int i = 0; i < num_symbols; ++i) {
        // Build the binary representation
        int level = nodes_[i].freq;
        if (last_level != level) {
            if (last_level != -1) {
                ++code;
                code <<= (level - last_level);
            }
            last_level = level;
        } else {
            ++code;
        }

        int symbol = nodes_[i].symbol;
        length_[symbol] = level;
        code_[symbol] = code;

        LOGV(10, "code:%s hex:%x level:%d symbol:%d\n", toBinary(code, level).c_str(), code, level, symbol);
    }
}

void HuffmanEncoder::walk(Node *n, int level) {
    if (n->symbol != -1) {
        n->freq = level;
        return;
    }

    walk(n->l, level + 1);
    walk(n->r, level + 1);
}

void HuffmanEncoder::writeTable(int num_symbols) { // write tree description to bit stream which the object releated.
    const int kSymBits = log2(max_symbols_); // we code every byte in the file, so, the max_symbols_ is 256, and we need 8 bits to represent it (00000000 ~ 11111111).
    writer_.writeBits(num_symbols - 1, kSymBits);  // step1: write how many symbols we coded

    // step2: write every symbol to bit stream (8 bits), 
    //        and the length of its huffman code (as we have limited the length of huffman code to 11 bits, so we only need to write 4 bits)
    for (int i = 0; i < num_symbols; ++i) { 
        writer_.writeBits(nodes_[i].symbol, kSymBits);
        writer_.writeBits(nodes_[i].freq - 1, 4);
    }

    // Byte align after the table
    writer_.finish();
}

void HuffmanEncoder::limitLength(int num_symbols) {
    // Limit the maximum code length
    int k = 0; // the k in here is not the real kraft number, it equals kraft number * 2^kMaxHuffCodeLength__
    int maxk = (1 << kMaxHuffCodeLength__) - 1; // Q: why we need minus one ??? if I delete the -1 in the rear, the kraft number seems to become all 1.
    for (int i = num_symbols - 1; i >= 0; --i) {
        nodes_[i].freq = std::min(nodes_[i].freq, kMaxHuffCodeLength__);
        k += 1 << (kMaxHuffCodeLength__ - nodes_[i].freq);
    }
    LOGV(10, "k before: %.6lf\n", k / double(maxk));
    for (int i = num_symbols - 1; i >= 0 && k > maxk; --i) {
        while (nodes_[i].freq < kMaxHuffCodeLength__) {
            ++nodes_[i].freq;
            k -= 1 << (kMaxHuffCodeLength__ - nodes_[i].freq);
        }
    }
    LOGV(10, "k pass1: %.6lf\n", k / double(maxk));
    for (int i = 0; i < num_symbols; ++i) {
        while (k + (1 << (kMaxHuffCodeLength__ - nodes_[i].freq)) <= maxk) {
            k += 1 << (kMaxHuffCodeLength__ - nodes_[i].freq);
            --nodes_[i].freq;
        }
    }
    LOGV(10, "k pass2: %.6lf\n", k / double(maxk));
}