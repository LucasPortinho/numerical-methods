#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../common_functions.c"

int convergence_coef(float** matrix, int i_size, int j_size) {
    float* past_coef = malloc(sizeof(float) * i_size);
    
    for (int i = 0; i < i_size; i++) {
        past_coef[i] = 1;
    }

    for (int i = 0; i < i_size; i++) {
        float coef_sum = 0;
        float coef_divisor = fabs(matrix[i][i]);
        
        for (int j = 0; j < j_size - 1; j++) {
            if (i != j) {
                coef_sum += fabs(matrix[i][j]) * past_coef[j];
            }
        }

        float coef = coef_sum / coef_divisor;
        
        if (coef >= 1) { 
            free(past_coef);
            return 0;
        }
        
        past_coef[i] = coef;
    }

    free(past_coef);
    return 1;
}

float* gauss_seidel(float** matrix, int i_size, int j_size, float* initial_values, float error) {
    if (!convergence_coef(matrix, i_size, j_size)) {
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
                    float multiplier = (i > j) ? new_values[j] : prev_values[j];
                    sum = sum - (matrix[i][j] * multiplier);
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

int main(int argc, char* argv[]) {
    const char* filename = (argc > 1) ? argv[1] : "sistema.txt";

    int i_size = 0, j_size = 0;
    float** matrix = read_linear_system(filename, &i_size, &j_size);
    if (!matrix) {
        printf("Falha ao carregar o sistema a partir de '%s'.\n", filename);
        return 1;
    }

    printf("--- Sistema Linear Carregado ('%s') ---\n", filename);
    print_matrix(matrix, i_size, j_size);

    float* initial_values = calloc(i_size, sizeof(float));
    float error = 0.000001;

    float* solution = gauss_seidel(matrix, i_size, j_size, initial_values, error);

    printf("\nSolucao encontrada:\n");
    for (int i = 0; i < i_size; i++) {
        printf("x%d = %.5f\n", i + 1, solution[i]);
    }

    free(initial_values);
    free(solution);
    free_matrix(matrix, i_size);

    return 0;
}