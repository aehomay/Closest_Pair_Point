#ifndef CLOSEST_PAIR_TOOLS_H_
#define CLOSEST_PAIR_TOOLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <math.h>
#include <string.h>




typedef struct {
	double x, y;
} point;

static double dist(point* a, point* b) {
	double dx = a->x - b->x, dy = a->y - b->y;
	return dx * dx + dy * dy;
}

static int cmp_dbl(double a, double b) {
	return a < b ? -1 : a > b ? 1 : 0;
}

static int cmp_x(const void *a, const void *b) {
	return cmp_dbl(((const point*) a)->x, ((const point*) b)->x);
}

static int cmp_y(const void *a, const void *b) {
	return cmp_dbl(((const point*) a)->y, ((const point*) b)->y);
}

#endif // end of CLOSEST_PAIR_TOOLS_H_
