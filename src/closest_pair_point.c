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
#include "data_set.h"

#define NP 1000000

int main(void) {

	int i;
	point a, b;

	point pts = malloc(sizeof(point_t) * NP);
	point* s_x = malloc(sizeof(point) * NP);
	point* s_y = malloc(sizeof(point) * NP);

	write_data_set("orginal.dat", NP);

	read_data_set("orginal.dat", s_x, pts);

	memcpy(s_y, s_x, sizeof(point) * NP);
	qsort(s_x, NP, sizeof(point), cmp_x);
	qsort(s_y, NP, sizeof(point), cmp_y);

	printf("min: %g; ", sqrt(closest(s_x, NP, s_y, NP, &a, &b)));

	free(s_x);
	free(s_y);

	return 1;
}
