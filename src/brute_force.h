#ifndef BRUTE_FORCE_H_
#define BRUTE_FORCE_H_
#endif
 
typedef struct { double x, y; } point_t, *point;
 


int cmp_x(const void *a, const void *b);
 
int cmp_y(const void *a, const void *b);
 
double brute_force(point* pts, int max_n, point *a, point *b);
 
double closest(point* sx, int nx, point* sy, int ny, point *a, point *b);

