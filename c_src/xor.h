#include "params.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifndef XOR_H
#define XOR_H

#define MAX_NUM_THREADS 48

// Global state variables for the generators
static uint64_t s[MAX_NUM_THREADS][2] = {
    {0xdeadbeef, 0xcafebabe},
    {0xfeedface, 0xabcdef01},
    {0x12345678, 0x87654321},
    {0xdeadcafe, 0xbeefc0de},
    {0xabcdef01, 0xfeedface},
    {0x87654321, 0x12345678},
    {0xbeefc0de, 0xdeadcafe},
    {0xcafebabe, 0xdeadbeef},
    {0xabcdef01, 0x87654321},
    {0xdeadcafe, 0x12345678},
    {0xbeefc0de, 0xfeedface},
    {0xdeadbeef, 0xcafebabe},
    {0x87654321, 0xabcdef01},
    {0x12345678, 0xdeadcafe},
    {0xfeedface, 0xbeefc0de},
    {0xcafebabe, 0xabcdef01},
    {0x12345678, 0xdeadcafe},
    {0x87654321, 0xbeefc0de},
    {0xabcdef01, 0xcafebabe},
    {0xbeefc0de, 0xdeadbeef},
    {0xcafebabe, 0x12345678},
    {0xfeedface, 0xdeadcafe},
    {0xbeefc0de, 0x87654321},
    {0xcafebabe, 0xdeadbeef},
    {0x12345678, 0xabcdef01},
    {0x87654321, 0xdeadcafe},
    {0xdeadbeef, 0xbeefc0de},
    {0xcafebabe, 0xabcdef01},
    {0x87654321, 0xdeadcafe},
    {0x12345678, 0xbeefc0de},
    {0xdeadbeef, 0xcafebabe},
    {0xabcdef01, 0x87654321},
    {0xbeefc0de, 0x12345678},
    {0xdeadcafe, 0xfeedface},
    {0xdeadbeef, 0xcafebabe},
    {0xfeedface, 0xabcdef01},
    {0x12345678, 0x87654321},
    {0xdeadcafe, 0xbeefc0de},
    {0xabcdef01, 0xfeedface},
    {0x87654321, 0x12345678},
    {0xbeefc0de, 0xdeadcafe},
    {0xcafebabe, 0xdeadbeef},
    {0xabcdef01, 0x87654321},
    {0xdeadcafe, 0x12345678},
    {0xbeefc0de, 0xfeedface},
    {0xdeadbeef, 0xcafebabe},
    {0x87654321, 0xabcdef01},
    {0x12345678, 0xdeadcafe}
};

// Generate a 64-bit pseudorandom number using xorshift128plus
uint64_t xorshift128plus(uint64_t* state) {
    uint64_t s1 = state[0];
    const uint64_t s0 = state[1];
    state[0] = s0;
    s1 ^= s1 << 23;
    state[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
    return state[1];
}

#endif