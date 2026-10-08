#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float* vet_difference(float* v1, float* v2, int n);
float biggest_module(float* values, int n);
float** sorted_matrix(float** matrix, int i_size, int j_size);

float** read_linear_system(const char* filename, int* i_size, int* j_size);
void free_matrix(float** matrix, int i_size);
void print_matrix(float** matrix, int i_size, int j_size);

#endif // COMMON_FUNCTIONS_H
