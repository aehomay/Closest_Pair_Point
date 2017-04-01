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
#include "timer.h"

#define RESULT_FILE_NAME "Result.dat"
#define DATASET_FILE_NAME "Orginal.dat"

void execute_brute_force();
double closest(point* sx, int nx, point* sy, int ny, point *a, point *b);


int main(void) {

	while (1) {

		printf(
				"To run Brute-force algorithm for closest-pair-of-pints press 'B'\n");
		printf(
				"To execute Divide-and-Conquer algorithm for closest-pair-of-pints press 'D'\n");
		printf(
				"To execute Parallel Divide-and-Conquer algorithm for closest-pair-of-pints press 'P'\n");
		printf("To see latest result file press 'R'\n");
		printf("To exit press 'Q'\n");

		char msg[] = "Selected functionality is not implemented yet.";
		char command = getchar();
		command = toupper(command);
		int exit = (command == 'Q');
		if (exit) {
			printf("\033[2J");
			break; //I will break the while loop
		}
		switch (command) {
		case 'B':
			execute_brute_force();
			break;
		case 'D':
			printf(msg);
			break;
		case 'P':
			printf(msg);
			break;
		case 'R':
			printf(msg);
			break;
		}

	}

	printf("\n*** You exit system successfully. ***\n");
	return 1;
}

void execute_brute_force() {

	long NP;
	point a, b;

	printf("\nTo run your algorithm please insert number of points:");
	scanf("%ld", &NP);

	start_watch(); //Start timer

	point pts = malloc(sizeof(point_t) * NP);
	point* s_x = malloc(sizeof(point) * NP);
	point* s_y = malloc(sizeof(point) * NP);

	//TODO: Don`t data create file if it is already exist
	generate_data_set(DATASET_FILE_NAME, NP);

	load_data_set(DATASET_FILE_NAME, s_x, pts);

	memcpy(s_y, s_x, sizeof(point) * NP);
	qsort(s_x, NP, sizeof(point), cmp_x);
	qsort(s_y, NP, sizeof(point), cmp_y);

	//TODO: Write the result into file and named Result.data
	double min_pair = closest(s_x, NP, s_y, NP, &a, &b);
	min_pair = sqrt(min_pair);

	free(s_x);
	free(s_y);

	unsigned long long t = stop_watch();

	write_into_file_format(RESULT_FILE_NAME, "Number of points: %ld\n"
			"Minimum distance is: %g\n"
			"Closest pair set is: {a(%f,%f), b(%f,%f)}\n"
			"Execution time: %llu ms\n", NP, min_pair, a->x, a->y, b->x, b->y, t);

	printf(
			"Brute force has run successfully to see the results check Result.dat .");
	getchar();
}
