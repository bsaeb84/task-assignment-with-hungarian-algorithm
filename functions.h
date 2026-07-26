//
// Created by Bahar Saeb on 03/07/2026.
//

#ifndef ALLOCATING_RESOURCES_FUNCTIONS_H
#define ALLOCATING_RESOURCES_FUNCTIONS_H

//the struct for assignments
typedef struct {
    int task_num;
    int resources_num;
    int n; //this is for the square  matrix
    char **task_names;
    char **resources_names;
    int **costs;
} Assignment;

int** createMatrix(int *task_num, int *num_resources, int *n_final);

void clear_buffer();

void remove_endline(char *str);

void get_name(char **name, int num, const char *label);

void intialize_assignment(Assignment* assignment);

void fill_names (Assignment* assignment);

void fill_matrix(Assignment *assignment);

void fill_cell(char *task_name, char *resources_name, int *cell);

void print_header(Assignment* assignment);

void print_matrix(Assignment* assignment);

void find_min_row(int *row, int n);

void step_1(Assignment* assignment);

void find_min_col(Assignment *assignment, int col_index);

void step_2(Assignment* assignment);

void find_zeros(Assignment *assignment, int zeros[][20]);
#endif //ALLOCATING_RESOURCES_FUNCTIONS_H
