#include "ising.h"

// Global state variables for the generators
static uint64_t s[2] = {0xdeadbeef, 0xcafebabe};

// Generate a 64-bit pseudorandom number using xorshift128plus
uint64_t xorshift128plus() {
    uint64_t s1 = s[0];
    const uint64_t s0 = s[1];
    s[0] = s0;
    s1 ^= s1 << 23;
    s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
    return s[1];
}


void update(const float temp, int grid[L][L])
{
    float expfs[2] = {expf(-(4/temp)), expf(-(8/temp))};
    for (unsigned int i = 0; i < HEIGHT; ++i) {
        for (unsigned int j = 0; j < WIDTH; ++j) {
            int spin_old = grid[i][j];
            int spin_new = (-1) * spin_old;

            // computing h_before
            int spin_neigh_n = grid[(i + 1) % L][j];
            int spin_neigh_e = grid[i][(j + 1) % L];
            int spin_neigh_w = grid[i][(j + L - 1) % L];
            int spin_neigh_s = grid[(i + L - 1) % L][j];
            int h_before = -spin_old * (spin_neigh_e+spin_neigh_n+spin_neigh_s+spin_neigh_w);

            // h after taking new spin
            int h_after = -spin_new * (spin_neigh_e+spin_neigh_n+spin_neigh_s+spin_neigh_w);
            
            int delta_E = h_after - h_before;
            
            int rand = xorshift128plus()%RAND_MAX;
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
