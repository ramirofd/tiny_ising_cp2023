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

size_t IDX(size_t x, size_t y) {
	return y * WIDTH + x;
}

void update(const float temp, int * grid)
{
    float expfs[2] = {expf(-(4/temp)), expf(-(8/temp))};
    for (unsigned int i = 0; i < HEIGHT; ++i) { //filas
        for (unsigned int j = 0; j < WIDTH; ++j) { //columnas
            int spin_old = grid[IDX(j,i)];
            int spin_new = (-1) * spin_old;

            // computing h_before
            int spin_neigh_n = grid[IDX(j,(i + 1) % HEIGHT)];
            int spin_neigh_e = grid[IDX((j + 1) % WIDTH, i)];
            int spin_neigh_w = grid[IDX((j + L - 1) % WIDTH, i)];
            int spin_neigh_s = grid[IDX(j, (i + L - 1) % HEIGHT)];
            int h_before = -spin_old * (spin_neigh_e+spin_neigh_n+spin_neigh_s+spin_neigh_w);

            // h after taking new spin
            int h_after = -spin_new * (spin_neigh_e+spin_neigh_n+spin_neigh_s+spin_neigh_w);
            
            int delta_E = h_after - h_before;
            
            int rand = xorshift128plus()%RAND_MAX;
            float p = rand / (float)RAND_MAX;
            if (delta_E <= 0 || p <= expfs[delta_E/4-1]) {
                grid[IDX(j,i)] = spin_new;
            }
        }
    }
}



double calculate(int * grid, int* M_max)
{
    int E = 0;
    for (unsigned int i = 0; i < HEIGHT; ++i) {
        for (unsigned int j = 0; j < WIDTH; ++j) {
            int spin = grid[IDX(j,i)];
            int spin_neigh_n = grid[IDX(j,(i + 1) % HEIGHT)];
            int spin_neigh_e = grid[IDX((j + 1) % WIDTH, i)];
            int spin_neigh_w = grid[IDX((j + L - 1) % WIDTH, i)];
            int spin_neigh_s = grid[IDX(j, (i + L - 1) % HEIGHT)];

            E += (spin * spin_neigh_n) + (spin * spin_neigh_e) + (spin * spin_neigh_w) + (spin * spin_neigh_s);
            *M_max += spin;
        }
    }
    return -((double)E / 2.0);
}
