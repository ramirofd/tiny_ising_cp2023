#include "params.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void xorshift128plus_init();
void update(const float temp, int grid[L][L]);
double calculate(int grid[L][L], int* M_max);
