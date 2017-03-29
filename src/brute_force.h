#ifndef BRUTE_FORCE_H_
#define BRUTE_FORCE_H_
#endif

typedef struct {
	double x, y;
} point_t, *point;

static double dist(point a, point b) {
	double dx = a->x - b->x, dy = a->y - b->y;
	return dx * dx + dy * dy;
}

static int cmp_dbl(double a, double b) {
	return a < b ? -1 : a > b ? 1 : 0;
}

static int cmp_x(const void *a, const void *b) {
	return cmp_dbl((*(const point*) a)->x, (*(const point*) b)->x);
}

static int cmp_y(const void *a, const void *b) {
	return cmp_dbl((*(const point*) a)->y, (*(const point*) b)->y);
}

double brute_force(point* pts, int max_n, point *a, point *b);

double closest(point* sx, int nx, point* sy, int ny, point *a, point *b);

