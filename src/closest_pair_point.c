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
#include <stdarg.h>
#include "brute_force.h"
#include "data_set.h"

#define NP 1000000

int main(void) {

	int i;
	point a, b;

	point pts = malloc(sizeof(point_t) * NP);
	point* s_x = malloc(sizeof(point) * NP);
	point* s_y = malloc(sizeof(point) * NP);

	//TODO: Don`t data create file if it is already exist
	generate_data_set("orginal.dat", NP);

	load_data_set("orginal.dat", s_x, pts);

	memcpy(s_y, s_x, sizeof(point) * NP);
	qsort(s_x, NP, sizeof(point), cmp_x);
	qsort(s_y, NP, sizeof(point), cmp_y);

	//TODO: Write the result into file and named Result.data
	double min_pair = sqrt(closest(s_x, NP, s_y, NP, &a, &b));


	write_into_file_format("Result.dat", "Minimum: %g points a(%f,%f) and b(%f,%f)\n",min_pair, a->x,
			a->y, b->x, b->y);

	free(s_x);
	free(s_y);

	return 1;
}
