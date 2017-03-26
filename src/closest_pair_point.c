/*
 ============================================================================
 Name        : ClosetPariPoint.c
 Author      : Aydin Homay
 Version     :
 Copyright   : @ Grid Computing Course FEUP 2017
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "brute_force.h"

#define NP 1000000

int main(void) {


    int i;
    point a, b;

    point pts  = malloc(sizeof(point_t) * NP);
    point* s_x = malloc(sizeof(point) * NP);
    point* s_y = malloc(sizeof(point) * NP);

    for(i = 0; i < NP; i++) {
            s_x[i] = pts + i;
            pts[i].x = 100 * (double) rand()/RAND_MAX;
            pts[i].y = 100 * (double) rand()/RAND_MAX;
    }

/*      printf("brute force: %g, ", sqrt(brute_force(s_x, NP, &a, &b)));
    printf("between (%f,%f) and (%f,%f)\n", a->x, a->y, b->x, b->y);        */

    memcpy(s_y, s_x, sizeof(point) * NP);
    qsort(s_x, NP, sizeof(point), cmp_x);
    qsort(s_y, NP, sizeof(point), cmp_y);

    printf("min: %g; ", sqrt(closest(s_x, NP, s_y, NP, &a, &b)));
    printf("point (%f,%f) and (%f,%f)\n", a->x, a->y, b->x, b->y);

    free(s_x);
    free(s_y);
    /* not freeing the memory, let OS deal with it.  Habit. */


	return EXIT_SUCCESS;
}
