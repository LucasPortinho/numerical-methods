#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int diagonal_dominant(float** matrix, int i_size, int j_size) {
    for (int i = 0; i < i_size; i++) {
        float coef_sum = 0;
        float coef_divisor = fabs(matrix[i][i]);

        for (int j = 0; j < j_size - 1; j++) {
            if (i != j) {
                coef_sum += fabs(matrix[i][j]);
            }
        }

        float coef = coef_sum / coef_divisor;

        if (coef > 1) return 0;
    }
    return 1;
}

float** sorted_matrix(float** matrix, int i_size, int j_size) {
    for (int i = 0; i < i_size; i++) {
        int max_row = i;
        float max_val = fabs(matrix[i][i]); 

        // maior valor em modulo da diagonal abaixo da linha atual
        for (int k = i + 1; k < i_size; k++) {
            if (fabs(matrix[k][i]) > max_val) {
                max_val = fabs(matrix[k][i]);
                max_row = k;
            }
        }
        
        if (max_row != i) {
            float* temp_row = matrix[i];
            matrix[i] = matrix[max_row];
            matrix[max_row] = temp_row;
        }
    }

    return matrix;
}

float* vet_difference(float* v1, float* v2, float n) {
    float* dif = malloc(sizeof(float) * n);
    for (int i = 0; i < n; i++) {
        dif[i] = v1[i] - v2[i];
    }

    return dif;
}

float biggest_module(float* values, int n) {
    float biggest = 0;
    for (int i = 0; i < n; i++) {
        if (fabs(values[i]) > biggest) biggest = fabs(values[i]);
    }
    return biggest;
}

float* gauss_jacobi(float** matrix, int i_size, int j_size, float* initial_values, float error)  {
    if (!diagonal_dominant(matrix, i_size, j_size)) {
        matrix = sorted_matrix(matrix, i_size, j_size);
    }

    float current_e = 0;
    
    float* prev_values = malloc(sizeof(float) * i_size);
    for(int i = 0; i < i_size; i++) prev_values[i] = initial_values[i];
    
    float* new_values = malloc(sizeof(float) * i_size);
    
    do {
        for (int i = 0; i < i_size; i++) {
            float sum = matrix[i][j_size - 1];
            float divisor = matrix[i][i];
            
            for (int j = 0; j < j_size - 1; j++) {
                if (i != j) {
                    sum = sum - (matrix[i][j] * prev_values[j]);
                }
            }

            new_values[i] = sum / divisor;
        }

        float* diff_vector = vet_difference(new_values, prev_values, i_size);
        current_e = biggest_module(diff_vector, i_size) / biggest_module(new_values, i_size);
        
        free(diff_vector);

        for (int i = 0; i < i_size; i++) {
            prev_values[i] = new_values[i];
        }
    }
    while (current_e > error);

    free(prev_values);

    return new_values;
}
