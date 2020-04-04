#ifndef DEBUG_H_
#define DEBUG_H_

#include <cstdio>
#include <cstdlib>

/**
 * @brief: assert, used for conditionary check
 * @param: [cond] the condition expression
 */ 
#define CHECK(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "%s: %d CHECK %s\n", __FILE__, __LINE__, #cond); \
        exit(1); \
    } \
} while (0);

constexpr int kDebugLevel = 0;

/**
 * @brief: used for debug info output
 * @param: [level] the log level, lower, more important
 * @param: [s] formatted string
 * @param: [...] formatted string param
 */ 
#define LOGV(level, s, ...) do { \
    if (level <= kDebugLevel) { \
        fprintf(stderr, "[%-60s : %4d] ", __FILE__, __LINE__); \
        fprintf(stderr, s, ##__VA_ARGS__); \
    } \
} while (0);

#define SYSTEM_CHECK

#endif // DEBUG_H_