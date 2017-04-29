#ifndef CLOSEST_PAIR_TOOLS_H_
#define CLOSEST_PAIR_TOOLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <math.h>
#include <string.h>
#include "mpi/mpi.h"

typedef struct {
	double x, y;
} point_t;

static double dist(point_t* a, point_t* b) {
	double dx = a->x - b->x, dy = a->y - b->y;
	return dx * dx + dy * dy;
}


static int cmp_dbl(double a, double b) {
	return a < b ? -1 : a > b ? 1 : 0;
}

static int cmp_x(const void *a, const void *b) {
	return cmp_dbl(((const point_t*) a)->x, ((const point_t*) b)->x);
}

static int cmp_y(const void *a, const void *b) {
	return cmp_dbl(((const point_t*) a)->y, ((const point_t*) b)->y);
}

#endif // end of CLOSEST_PAIR_TOOLS_H_
