#include "params.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef enum {RED, BLACK} color;

void xorshift128plus_init();
void update_rb(color col, const float temp, int read[HEIGHT][WIDTH], int write[HEIGHT][WIDTH]);
int calculate_rb(color col, int read[HEIGHT][WIDTH], int write[HEIGHT][WIDTH], int* M_max);
void update(const float temp, int read[HEIGHT][WIDTH], int write[HEIGHT][WIDTH]);
double calculate(int read[HEIGHT][WIDTH], int write[HEIGHT][WIDTH], int* M_max);
