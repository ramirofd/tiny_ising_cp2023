#include <stdio.h>
#include <stdint.h>
#include <limits.h>

unsigned int lcg(unsigned int seed) {
    static const unsigned int a = 1664525;
    static const unsigned int c = 1013904223;
    static unsigned int prev = 0;

    if (seed != 0) {
        prev = seed;
    }

    prev = (a * prev + c) % UINT_MAX;
    return prev;
}

int main() {
    unsigned int seed = 12345;
    for (int i = 0; i < 10; i++) {
        printf("%u\n", lcg(seed));
    }
}