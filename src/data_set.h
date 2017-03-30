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

void generate_data_set(const char* file, long num_point);
void pars_from_file(FILE* fp, char delimate, char* p_buff, char* buff);
void write_into_file(const char* file, char* buff);
void write_into_file_double_pair(FILE* fp,double x, double y);
void write_into_file_string(FILE* fp, char* str);
void write_into_file_long(FILE* fp, long num_point);
void write_into_file_decimal(FILE* fp, double num_point);
void write_into_file_format(const char* file,const char* format, ...);


void load_data_set(const char* file, point* s_x, point pts) {

	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		printf("Error opening file!\n");
		exit(8);
	}

	char buff[256];
	long num_point;
	char c;

	// Read the file till the EOF (end of file) by starting from [
	while ((c = fgetc(fp)) != EOF) {

		//Reset pointer to buffer to the first location of buffer in each iteration
		char* p_buff = buff;

		//Read number of points and initialise num_point
		pars_from_file(fp,']', p_buff, buff);
		num_point = strtol(buff, &p_buff, 10);

		for (long i = 0; i < num_point; i++) {

			s_x[i] = pts + i;

			//Read X
			pars_from_file(fp,',', p_buff, buff);
			pts[i].x = strtod(buff, &p_buff);

			//Read Y
			pars_from_file(fp,'\n', p_buff, buff);
			pts[i].y = strtod(buff, &p_buff);

		}
	}

	fclose(fp);
}

void write_into_file_format(const char* file,const char* format, ...)
{
	FILE* fp = fopen(file, "w");

		if (fp == NULL) {
			printf("Error opening file!\n");
			exit(1);
		}


	va_list args;
	va_start(args,format);
	vfprintf(fp,format,args);
	va_end(args);
}

void pars_from_file(FILE* fp, char delimate, char* p_buff, char* buff) {
	char c = fgetc(fp);
	p_buff = buff;
	while (c != delimate && c != EOF) {
		*p_buff = c;
		p_buff++;
		c = fgetc(fp);
	}
}

void generate_data_set(const char* file, long num_point) {
	FILE* fp = fopen(file, "w");

	if (fp == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	write_into_file_long(fp,num_point);

	typedef struct {
		double x, y;
	} point, *p_ptr;
	p_ptr pts = malloc(sizeof(point) * num_point);

	for (long i = 0; i < num_point; i++) {
		pts[i].x = 100 * (double) rand() / RAND_MAX;
		pts[i].y = 100 * (double) rand() / RAND_MAX;
		write_into_file_double_pair(fp, pts[i].x, pts[i].y);
	}
	fclose(fp);
}

void write_into_file(const char* file, char* buff) {
	FILE* fp = fopen(file, "w");

	if (fp == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	write_into_file_string(fp,buff);
	fclose(fp);
}

void write_into_file_double_pair(FILE* fp,double x, double y) {

	fprintf(fp, "%f,%f\n", x, y); //[x,y]
}

void write_into_file_string(FILE* fp, char* str) {
	fprintf(fp, "[%s]\n", str);
}

void write_into_file_long(FILE* fp, long num_point) {
	fprintf(fp, "[%ld]\n", num_point);
}

void write_into_file_decimal(FILE* fp, double num_point) {
	fprintf(fp, "[%g]\n", num_point);
}
