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


void update_rb(color col, const float temp, int read[HEIGHT][WIDTH], int write[HEIGHT][WIDTH])
{
    float expfs[2] = {expf(-(4/temp)), expf(-(8/temp))};
    int side_shift = col == RED ? -1 : 1;

    for (unsigned int i = 0; i < HEIGHT; ++i, side_shift = -side_shift) {
        for (unsigned int j = 0; j < WIDTH; ++j) {
            int spin_old = write[i][j];
            int spin_new = (-1) * spin_old;

            // computing h_before
            int spin_neigh_n = read[i][(j - 1 + HEIGHT) % HEIGHT];
            int spin_neigh_e = read[i][j];
            int spin_neigh_w = read[(i + side_shift + WIDTH) % WIDTH][j];
            int spin_neigh_s = read[i][(j + 1) % HEIGHT];
            int h_before = -spin_old * (spin_neigh_e+spin_neigh_n+spin_neigh_s+spin_neigh_w);

            // h after taking new spin
            int h_after = -spin_new * (spin_neigh_e+spin_neigh_n+spin_neigh_s+spin_neigh_w);
            
            int delta_E = h_after - h_before;
            
            int rand = xorshift128plus()%RAND_MAX;
            float p = rand / (float)RAND_MAX;
            if (delta_E <= 0 || p <= expfs[delta_E/4-1]) {
                write[i][j] = spin_new;
            }
        }
    }
}



int calculate_rb(color col, int read[HEIGHT][WIDTH], int write[HEIGHT][WIDTH], int* M_max)
{
    int E = 0;
    int side_shift = col == RED ? -1 : 1;
    for (unsigned int i = 0; i < HEIGHT; ++i, side_shift = -side_shift) {
        for (unsigned int j = 0; j < WIDTH; ++j) {
            int spin = write[i][j];
            int spin_neigh_n = read[i][(j - 1 + HEIGHT) % HEIGHT];
            int spin_neigh_e = read[i][j];
            int spin_neigh_w = read[(i + side_shift + WIDTH) % WIDTH][j];
            int spin_neigh_s = read[i][(j + 1) % HEIGHT];

            E += (spin * spin_neigh_n) + (spin * spin_neigh_e) + (spin * spin_neigh_w) + (spin * spin_neigh_s);
            *M_max += spin;
        }
    }
    return E;
}


void update(const float temp, 
            int grid_r[HEIGHT][WIDTH], 
            int grid_b[HEIGHT][WIDTH]) {
	update_rb(RED, temp, grid_b, grid_r);
	update_rb(BLACK, temp, grid_r, grid_b);
}

double calculate(int grid_r[HEIGHT][WIDTH], 
                 int grid_b[HEIGHT][WIDTH], 
                 int* M_max) {
	int E = 0;
	E += calculate_rb(RED, grid_b, grid_r, M_max);
	E += calculate_rb(BLACK, grid_r, grid_b, M_max);
	return - (double) E / 2.0;
}