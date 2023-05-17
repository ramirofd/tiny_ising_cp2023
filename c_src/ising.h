#include "params.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef enum {RED, BLACK} color;

void xorshift128plus_init();
size_t idx(size_t i, size_t j);
void update_rb(color col, const float temp, const int * restrict read, int * restrict write);
int calculate_rb(color col, const int * restrict read, const int * restrict write, int * restrict M_max);
void update(const float temp, int * restrict read, int * restrict write);
double calculate(int * restrict read, int * restrict write, int * restrict M_max);
