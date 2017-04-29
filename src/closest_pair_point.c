/*
 ============================================================================
 Name        : ClosetPariPoint.c
 Author      : Aydin Homay
 Version     :
 Copyright   : @ Grid Computing Course FEUP 2017
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdarg.h>
#include "../src/tools/data_set.h"
#include "../src/tools/timer.h"
#include "../src/tools/closest_pair_tools.h"

#define RESULT_FILE_NAME "Result.dat"
#define DATASET_FILE_NAME "Orginal.dat"
#define MAX_LENGTH 1000000

#define EXCEP_01 "The selected functionality is not implemented yet."
#define EXCEP_02 "Brute force has not been run successfully check data-set file creation."
#define EXCEP_03 "Process is cancelled by user."

#define SUCC_MSG_01 "\nBrute force has been run successfully to see the results check Result.dat."
#define SUCC_MSG_02 "\n*** You exit system successfully. ***\n"
#define SUCC_MSG_03 "\nData-set is generated successfully."

void execute_brute_force();
double brute_force(point* sx, int nx, point* sy, int ny, point *a, point *b);
int generate_orginal_data(const char* file);
void print_results(const char* file);

int main(void) {

	printf("To generate data-set press 'I', Note: the file name will be {%s}\n",
	DATASET_FILE_NAME);
	printf(
			"To run Brute-force algorithm for closest-pair-of-pints press 'B'\n");
	printf(
			"To execute Divide-and-Conquer algorithm for closest-pair-of-pints press 'D'\n");
	printf(
			"To execute Parallel Divide-and-Conquer algorithm for closest-pair-of-pints press 'P'\n");
	printf("To see latest result file press 'O'\n");
	printf("To exit press 'Q'\n");

	while (1) {

		char command = getchar();
		command = toupper(command);
		int exit = (command == 'Q');
		if (exit)
			break; //I will break the while loop

		switch (command) {
		case 'I':
			if (generate_orginal_data(DATASET_FILE_NAME) == 0)
				printf(EXCEP_03);
			else
				printf(SUCC_MSG_03);
			break;
		case 'B':
			execute_brute_force();
			break;
		case 'D':
			printf(EXCEP_01);
			break;
		case 'P':
			printf(EXCEP_01);
			break;
		case 'R':
			printf(EXCEP_01);
			break;
		}
	}

	printf(SUCC_MSG_02);
	EXIT_SUCCESS;
	return 1;
}

void execute_brute_force() {

	long length = read_data_set_header(DATASET_FILE_NAME);

	if (length == 0)
		return;

	point a, b;

	start_watch(); //Start timer

	point pts = malloc(sizeof(point_t) * length);
	point* s_x = malloc(sizeof(point) * length);
	point* s_y = malloc(sizeof(point) * length);

	load_data_set(DATASET_FILE_NAME, s_x, pts);

	memcpy(s_y, s_x, sizeof(point) * length);
	qsort(s_x, length, sizeof(point), cmp_x);
	qsort(s_y, length, sizeof(point), cmp_y);

	double min_pair = brute_force(s_x, length, s_y, length, &a, &b);
	min_pair = sqrt(min_pair);

	free(s_x);
	free(s_y);

	unsigned long long t = stop_watch();

	write_into_file_format(RESULT_FILE_NAME, "Number of points: %ld\n"
			"Minimum distance is: %g\n"
			"Closest pair set is: {a(%f,%f), b(%f,%f)}\n"
			"Execution time: %llu ms\n", length, min_pair, a->x, a->y, b->x,
			b->y, t);

	printf(
			"Brute force has run successfully to see the results check Result.dat .");
}

int generate_orginal_data(const char* file) {

	long length = 0;
	printf("\nTo run your algorithm please insert number of points:");
	scanf("%ld", &length);
	if (length > MAX_LENGTH) {
		char command = NULL;
		printf(
				"\nThe number of points can not be more than {%ld}, to continue press 'y' to cancel press 'n':");
		scanf("%c", command);
		if (toupper(command) == 'N')
			return 0;
		else
			length = MAX_LENGTH;
	}
	//TODO: Don`t data create file if it is already exist
	generate_data_set(DATASET_FILE_NAME, length);
	return length;
}

void print_results(const char* file) {
}
