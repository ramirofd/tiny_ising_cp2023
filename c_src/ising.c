#include "ising.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

// #define SEED 12345

uint64_t state[2] = {0xdeadbeef, 0xcafebabe};

void xorshift128plus() {
    uint64_t s1 = state[0];
    const uint64_t s0 = state[1];
    state[0] = s0;
    s1 ^= s1 << 23;
    state[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
}

unsigned int LCG(unsigned int seed) {
    static const unsigned int a = 1664525;
    static const unsigned int c = 1013904223;
    static unsigned int prev = 0;

    if (seed != 0) {
        prev = seed;
    }

    prev = (a * prev + c) % RAND_MAX;
    return prev;
}

void update(const float temp, int grid[L][L])
{
    // int seed = SEED;
    // typewriter update
    float expfs[2] = {expf(-(4/temp)), expf(-(8/temp))};
    for (unsigned int i = 0; i < L; ++i) {
        for (unsigned int j = 0; j < L; ++j) {
            int spin_old = grid[i][j];
            int spin_new = (-1) * spin_old;

            // computing h_before
            int spin_neighbours = grid[(i + L - 1) % L][j]
                                + grid[i][(j + 1) % L]
                                + grid[i][(j + L - 1) % L]
                                + grid[(i + 1) % L][j];
            int h_before = -spin_old * spin_neighbours;

            // h after taking new spin
            int h_after = -spin_new * spin_neighbours;
            
            int delta_E = h_after - h_before;

            // seed = LCG(seed);
            // float p = seed / (float)RAND_MAX;
            
            // float p = rand() / (float)RAND_MAX;
            xorshift128plus();
            int rand = state[1]%RAND_MAX;
            float p = rand / (float)RAND_MAX;
            if (delta_E <= 0 || p <= expfs[delta_E/4-1]) {
                grid[i][j] = spin_new;
            }
        }
    }
}


double calculate(int grid[L][L], int* M_max)
{
    int E = 0;
    for (unsigned int i = 0; i < L; ++i) {
        for (unsigned int j = 0; j < L; ++j) {
            int spin = grid[i][j];
            int spin_neigh_n = grid[(i + 1) % L][j];
            int spin_neigh_e = grid[i][(j + 1) % L];
            int spin_neigh_w = grid[i][(j + L - 1) % L];
            int spin_neigh_s = grid[(i + L - 1) % L][j];

            E += (spin * spin_neigh_n) + (spin * spin_neigh_e) + (spin * spin_neigh_w) + (spin * spin_neigh_s);
            *M_max += spin;
        }
    }
    return -((double)E / 2.0);
}
