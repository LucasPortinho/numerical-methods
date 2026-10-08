#include <stdlib.h>
#include <math.h>
#include "common_functions.h"

float* vet_difference(float* v1, float* v2, int n) {
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

float** read_linear_system(const char* filename, int* i_size, int* j_size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    int rows = 0, cols = 0;
    char header[256];
    if (!fgets(header, sizeof(header), file)) {
        fclose(file);
        return NULL;
    }

    int read_count = sscanf(header, "%d %d", &rows, &cols);
    if (read_count == 1) {
        cols = rows + 1; 
    } else if (read_count < 1 || rows <= 0 || cols <= 0) {
        printf("Formato invalido nas dimensoes do sistema.\n");
        fclose(file);
        return NULL;
    }

    float** matrix = malloc(sizeof(float*) * rows);
    if (!matrix) {
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(sizeof(float) * cols);
        if (!matrix[i]) {
            for (int k = 0; k < i; k++) free(matrix[k]);
            free(matrix);
            fclose(file);
            return NULL;
        }
        for (int j = 0; j < cols; j++) {
            if (fscanf(file, "%f", &matrix[i][j]) != 1) {
                printf("Erro ao ler elemento da linha %d, coluna %d.\n", i, j);
                for (int k = 0; k <= i; k++) free(matrix[k]);
                free(matrix);
                fclose(file);
                return NULL;
            }
        }
    }

    fclose(file);
    *i_size = rows;
    *j_size = cols;
    return matrix;
}

void free_matrix(float** matrix, int i_size) {
    if (!matrix) return;
    for (int i = 0; i < i_size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void print_matrix(float** matrix, int i_size, int j_size) {
    for (int i = 0; i < i_size; i++) {
        for (int j = 0; j < j_size; j++) {
            printf("%8.3f ", matrix[i][j]);
        }
        printf("\n");
    }
}
