/**
 * author: sjinglong
 */ 

#include <iostream>

#include "./tools/checker.h"

int main(int argc, char **argv) {

#ifdef SYSTEM_CHECK
    check_all();
#endif

    return 0;
}