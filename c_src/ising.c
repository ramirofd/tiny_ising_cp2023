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

size_t idx(size_t x, size_t y) {
    return y * WIDTH + x;
}

// void update_cell(const int *restrict read, int * restrict write, int i, int j, int neigh, float expfs[2]){
            
// }

void update_rb(color col, const float temp, const int * restrict read, int * restrict write)
{
    float expfs[2] = {expf(-(4/temp)), expf(-(8/temp))};
    int side_shift = col == RED ? -1 : 1;

    for (unsigned int y = 1; y < HEIGHT-1; ++y, side_shift = -side_shift) {
        for (unsigned int x = 1; x < WIDTH-1; ++x) {
            int spin_old = write[idx(x, y)];
			int spin_new = -spin_old;

			// computing h_before
			int spin_neigh_up   = read[idx(x, (y - 1 + HEIGHT) % HEIGHT)];
			int spin_neigh_same = read[idx(x, y)];
			int spin_neigh_side = read[idx((x + side_shift + WIDTH), y)];
			int spin_neigh_down = read[idx(x, (y + 1) % HEIGHT)];

			int h_before = - (spin_old*spin_neigh_up)   - (spin_old*spin_neigh_same)
				   - (spin_old*spin_neigh_side) - (spin_old*spin_neigh_down);

			// h after taking new spin
			int h_after = - (spin_new*spin_neigh_up)   - (spin_new*spin_neigh_same)
			              - (spin_new*spin_neigh_side) - (spin_new*spin_neigh_down);

			int delta_E = h_after - h_before;
            int rand = xorshift128plus()%RAND_MAX;
            float p = rand / (float)RAND_MAX;
            if (delta_E <= 0 || p <= expfs[delta_E/4-1]) {
                write[idx(x,y)] = spin_new;
            }
        }
    }
}



int calculate_rb(color col, const int * restrict read, const int * restrict write, int * restrict M_max)
{
    int E = 0;
    int side_shift = col == RED ? -1 : 1;
    for (unsigned int y = 0; y < HEIGHT; ++y, side_shift = -side_shift) {
        for (unsigned int x = 0; x < WIDTH; ++x) {
            int spin = write[idx(x,y)];
            int spin_neigh_n = read[idx(x,(y - 1 + HEIGHT) % HEIGHT)];
            int spin_neigh_e = read[idx(x,y)];
            int spin_neigh_w = read[idx((x + side_shift + WIDTH) % WIDTH,y)];
            int spin_neigh_s = read[idx(x,(y + 1) % HEIGHT)];

            E += (spin * spin_neigh_n) + (spin * spin_neigh_e) + (spin * spin_neigh_w) + (spin * spin_neigh_s);
            *M_max += spin;
        }
    }
    return E;
}


void update(const float temp, int * restrict grid_r, int * restrict grid_b) 
{
	update_rb(RED, temp, grid_b, grid_r);
	update_rb(BLACK, temp, grid_r, grid_b);
}

double calculate(int * restrict grid_r, int * restrict grid_b, int * restrict M_max) {
	int E = 0;
	E += calculate_rb(RED, grid_b, grid_r, M_max);
	E += calculate_rb(BLACK, grid_r, grid_b, M_max);
	return - (double) E / 2.0;
}