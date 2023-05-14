#include "params.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void xorshift128plus_init();
size_t IDX(size_t y, size_t x);
void update(const float temp, int * grid);
double calculate(int * grid, int* M_max);
