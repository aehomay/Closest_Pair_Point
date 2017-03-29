/*
 ============================================================================
 Name        : data_set.c
 Author      : Aydin Homay
 Version     :
 Copyright   : @ Grid Computing Course FEUP 2017
 Description : To generate data set and w/r from/to file
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

void read_data_set(const char* file, point* s_x, point pts) {

	fp = fopen(file, "r");
	if (fp == NULL) {
		printf("Error opening file!\n");
		exit(8);
	}

	char buff[256];
	char* p_buff;
	long num_point;
	char c;

	// Read the file till the EOF (end of file) by starting from [
	while ((c = fgetc(fp)) != EOF) {

		//Reset pointer to buffer to the first location of buffer in each iteration
		char* p_buff = buff;

		//Read number of points and initialise num_point
		pars_from_file(']', p_buff, buff, fp);
		num_point = strtol(buff, &p_buff, 10);

		for (long i = 0; i < num_point; i++) {

			s_x[i] = pts + i;

			//Read X
			pars_from_file(',', p_buff, buff, fp);
			pts[i].x = strtod(buff, &p_buff);

			//Read Y
			pars_from_file('\n', p_buff, buff, fp);
			pts[i].y = strtod(buff, &p_buff);

		}
	}

	fclose(fp);
}

inline void pars_from_file(char delimate, char* p_buff, char* buff, FILE* f) {
	char c = fgetc(f);
	p_buff = buff;
	while (c != delimate && c != EOF) {
		*p_buff = c;
		p_buff++;
		c = fgetc(f);
	}
}

void write_data_set(const char* file, long num_point) {
	fp = fopen(file, "w");

	if (fp == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	write_in_file_header(num_point);

	typedef struct {
		double x, y;
	} point, *p_ptr;
	p_ptr pts = malloc(sizeof(point) * num_point);

	for (long i = 0; i < num_point; i++) {
		pts[i].x = 100 * (double) rand() / RAND_MAX;
		pts[i].y = 100 * (double) rand() / RAND_MAX;
		write_in_file_points(file, pts[i].x, pts[i].y);
	}
	fclose(fp);
}

inline write_in_file_points(double x, double y) {

	fprintf(fp, "%f,%f\n", x, y); //[x,y]
}

inline write_in_file_header(long num_point) {
	fprintf(fp, "[%ld]\n", num_point);
}

