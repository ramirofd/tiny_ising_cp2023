/*
 * Tiny Ising model.
 * Loosely based on "q-state Potts model metastability
 * study using optimized GPU-based Monte Carlo algorithms",
 * Ezequiel E. Ferrero, Juan Pablo De Francesco, Nicolás Wolovick,
 * Sergio A. Cannas
 * http://arxiv.org/abs/1101.0876
 * 
 * Debugging: Ezequiel Ferrero
 * Red-Black: Carlos Bederián
 */

#include <stdlib.h> // rand()
#include <math.h> // expf()
#include <stdio.h> // printf()
#include <time.h> // time()
#include <assert.h>
#include <limits.h> // UINT_MAX
#include <omp.h> // omp_get_wtime()

#ifndef L
#define L 128 // linear system size
#endif

#ifndef SAMPLES
#define SAMPLES 1 // number of samples
#endif

#ifndef TEMP_MIN
#define TEMP_MIN 0.9f // minimum temperature
#endif

#ifndef TEMP_MAX
#define TEMP_MAX 1.35f // maximum temperature
#endif

#ifndef DELTA_TEMP
#define DELTA_TEMP 0.05f // temperature step
#endif

#ifndef TRAN
#define TRAN 20 // equilibration time
#endif

#ifndef TMAX
#define TMAX 800 // measurement time
#endif

#ifndef DELTA_T
#define DELTA_T 5 // sampling period for energy and magnetization
#endif

// Functions

// Internal definitions and functions
// out vector size, it is +1 since we reach TEMP_
#define NPOINTS (1+(int)((TEMP_MAX-TEMP_MIN)/DELTA_TEMP))
#define N (L*L) // system size
#define SEED (time(NULL)) // random seed
#define WIDTH (L/2)
#define HEIGHT (L)

typedef enum { RED, BLACK } grid_color;

// temperature, E, E^2, E^4, M, M^2, M^4
struct statpoint {
	double t;
	double e; double e2; double e4;
	double m; double m2; double m4;
};


/***
 * Functions
 ***/

static
size_t
idx(size_t x, size_t y) {
	return y * WIDTH + x;
}

static
void
update_rb(grid_color color,
          const float temp,
          const int * restrict read,
          int * restrict write) {

	int side_shift = color == RED ? -1 : 1;

	for (int y = 0; y < HEIGHT; ++y, side_shift = -side_shift) {
		for (int x = 0; x < WIDTH; ++x) {
			int spin_old = write[idx(x, y)];
			int spin_new = -spin_old;

			// computing h_before
			int spin_neigh_up   = read[idx(x, (y - 1 + HEIGHT) % HEIGHT)];
			int spin_neigh_same = read[idx(x, y)];
			int spin_neigh_side = read[idx((x + side_shift + WIDTH) % WIDTH, y)];
			int spin_neigh_down = read[idx(x, (y + 1) % HEIGHT)];

			int h_before = - (spin_old*spin_neigh_up)   - (spin_old*spin_neigh_same)
				   - (spin_old*spin_neigh_side) - (spin_old*spin_neigh_down);

			// h after taking new spin
			int h_after = - (spin_new*spin_neigh_up)   - (spin_new*spin_neigh_same)
			              - (spin_new*spin_neigh_side) - (spin_new*spin_neigh_down);

			int delta_E = h_after - h_before;
			float p = rand()/(float)RAND_MAX;
			if (delta_E<=0 || p<=expf(-delta_E/temp)) {
				write[idx(x, y)] = spin_new;
			}
		}
	}
}

static
void
update(const float temp,
       int * restrict grid_r,
       int * restrict grid_b) {
	update_rb(RED, temp, grid_b, grid_r);
	update_rb(BLACK, temp, grid_r, grid_b);
}


static
int
calculate_rb(grid_color color,
             const int * restrict neigh,
             const int * restrict grid,
             int * restrict M_max) {

	int E = 0;
	int side_shift = color == RED ? -1 : 1;

	for (int y = 0; y < HEIGHT; ++y, side_shift = -side_shift) {
		for (int x = 0; x < WIDTH; ++x) {
			int spin = grid[idx(x, y)];
			int spin_neigh_up   = neigh[idx(x, (y - 1 + HEIGHT) % HEIGHT)];
			int spin_neigh_same = neigh[idx(x, y)];
			int spin_neigh_side = neigh[idx((x + side_shift + WIDTH) % WIDTH, y)];
			int spin_neigh_down = neigh[idx(x, (y + 1) % HEIGHT)];

			E += (spin * spin_neigh_up)   + (spin * spin_neigh_same) +
			     (spin * spin_neigh_side) + (spin * spin_neigh_down);
			*M_max += spin;
		}
	}

	return E;
}

static
double
calculate(const int * restrict grid_r,
          const int * restrict grid_b,
          int * restrict M_max) {
	int E = 0;
	E += calculate_rb(RED, grid_b, grid_r, M_max);
	E += calculate_rb(BLACK, grid_r, grid_b, M_max);
	return - (double) E / 2.0;
}


static
void
cycle(int * restrict grid_r,
      int * restrict grid_b,
      const double min, const double max,
      const double step, const unsigned int calc_step,
      struct statpoint stats[]) {

	unsigned int index = 0;
	int modifier = 0;
	double temp = 0.0;

	assert((0<step && min<=max) || (step<0 && max<=min));
	modifier = (0<step)?1:-1;

	for (index=0, temp=min;
	     modifier*temp<=modifier*max;
	     ++index, temp+=step) {

		// equilibrium phase
		for (unsigned int j=0; j<TRAN; ++j) {
			update(temp, grid_r, grid_b);
		}

		// measurement phase
		unsigned int measurements = 0;
		double e=0.0, e2=0.0, e4=0.0, m=0.0, m2=0.0, m4=0.0;
		for (unsigned int j=0; j<TMAX; ++j) {
			update(temp, grid_r, grid_b);
			if (j%calc_step==0) {
				double energy = 0.0, mag = 0.0;
				int M_max = 0;
				energy = calculate(grid_r, grid_b, &M_max);
				mag = abs(M_max)/(float)N;
				e  += energy;
				e2 += energy*energy;
				e4 += energy*energy*energy*energy;
				m  += mag;
				m2 += mag*mag;
				m4 += mag*mag*mag*mag;
				++measurements;
			}
		}
		assert(index<NPOINTS);
		stats[index].t = temp;
		stats[index].e += e/measurements;
		stats[index].e2 += e2/measurements;
		stats[index].e4 += e4/measurements;
		stats[index].m += m/measurements;
		stats[index].m2 += m2/measurements;
		stats[index].m4 += m4/measurements;
	}
}


static
void
sample(int * restrict grid_r,
       int * restrict grid_b,
       struct statpoint stat[]) {
	// clear the grid
	for (unsigned int y = 0; y < HEIGHT; ++y) {
		for (unsigned int x = 0; x < WIDTH; ++x) {
			grid_r[idx(x, y)] = 1;
			grid_b[idx(x, y)] = 1;
		}
	}

	// temperature increasing cycle
	cycle(grid_r, grid_b, TEMP_MIN, TEMP_MAX, DELTA_TEMP, DELTA_T, stat);
}


int
main(void)
{
	// the grid
	size_t size = HEIGHT * WIDTH * sizeof(int);
	int * grid_r = malloc(size);
	int * grid_b = malloc(size);

	// the stats
	struct statpoint stat[NPOINTS];
	for (unsigned int i=0; i<NPOINTS; ++i) {
			stat[i].t = 0.0;
			stat[i].e = stat[i].e2 = stat[i].e4 = 0.0;
			stat[i].m = stat[i].m2 = stat[i].m4 = 0.0;
	}
	double start = 0.0;
	double elapsed = 0.0;

	// parameter checking
	assert(TEMP_MIN<=TEMP_MAX);
	assert(0<DELTA_T && DELTA_T<TMAX); // at least one calculate()
	assert(TMAX%DELTA_T==0); // take equidistant calculate()
	assert((L*L/2)*4L<UINT_MAX); // max energy, that is all spins are the same, fits into a ulong

	// print header
	printf("# L: %i\n", L);
	printf("# Number of Samples: %i\n", SAMPLES);
	printf("# Minimum Temperature: %f\n", TEMP_MIN);
	printf("# Maximum Temperature: %f\n", TEMP_MAX);
	printf("# Temperature Step: %.12f\n", DELTA_TEMP);
	printf("# Equilibration Time: %i\n", TRAN);
	printf("# Measurement Time: %i\n", TMAX);
	printf("# Data Acquiring Step: %i\n", DELTA_T);
	printf("# Number of Points: %i\n", NPOINTS);

	// configure RNG
	srand(SEED);

	// start timer
	start = omp_get_wtime();

	for (unsigned int i=0; i<SAMPLES; ++i) {
		sample(grid_r, grid_b, stat);
	}

	// stop timer
	elapsed = omp_get_wtime()-start;
	printf("# Total Simulation Time (sec): %lf\n", elapsed);

	printf("# Temp\tE\tE^2\tE^4\tM\tM^2\tM^4\n");
	for (unsigned int i=0; i<NPOINTS; ++i) {
		printf ("%lf\t%.10lf\t%.10lf\t%.10lf\t%.10lf\t%.10lf\t%.10lf\n",
			stat[i].t,
			stat[i].e/((double)N*SAMPLES),
			stat[i].e2/((double)N*N*SAMPLES),
			stat[i].e4/((double)N*N*N*N*SAMPLES),
			stat[i].m/SAMPLES,
			stat[i].m2/SAMPLES,
			stat[i].m4/SAMPLES);
	}

	free(grid_r);
	free(grid_b);

	return 0;
}