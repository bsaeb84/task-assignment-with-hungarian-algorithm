#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define NUM_TASKS 20
#define NUM_RESOURCES 20
#define MAX_NAME_LENGTH 100
#include "functions.h"

#include <string.h>





int** createMatrix (int *task_num, int *num_resources, int *n_final) {
    do {
        printf("Enter number of tasks: \n");
        scanf("%d", task_num);
        if (*task_num > NUM_TASKS)
            printf("Number of tasks exceeds %d\n Try again:\n", NUM_TASKS);
        else if (*task_num <= 0) {
            printf("Number of tasks can't be less than 0\n Try again:\n");
        }
    }while (*task_num <= 0 || *task_num > NUM_TASKS);

    do {
        printf("Enter number of resources: \n");
        scanf("%d", num_resources);
        if (*num_resources > NUM_TASKS)
            printf("Number of resources exceeds %d\n Try again:\n", NUM_TASKS);
        else if (*num_resources <= 0) {
            printf("Number of resources can't be less than 0\n Try again:\n");
        }
    }while (*num_resources <= 0 || *num_resources > NUM_RESOURCES);

    //this reduces memory bug
    int n = (*task_num > *num_resources) ? *task_num : *num_resources;  // figure out which one is bigger
    *n_final = n;

    int **matrix = malloc(n * sizeof(int*)); //make pointer array for rows

    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(int));
    }

    return matrix;
}

char** create_task_array_names(int n) {
    char **names = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        names[i] = malloc(MAX_NAME_LENGTH * sizeof(char));
    }
    return names;

}
void clear_buffer() {
    int input;
    while ((input = getchar()) != '\n' && input != EOF);
}

void remove_endline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

//function to get name of either resources or tasks
void get_name(char **name, int num, const char *label) {
    for (int i=0; i<num; i++) {
        printf("Enter name for %s number: %d\n", label, i+1);
        fgets(*(name+i), MAX_NAME_LENGTH, stdin);
        remove_endline(*(name+i));
    }
}

void intialize_assignment(Assignment* assignment) {

    assignment->costs = createMatrix(&assignment->task_num, &assignment->resources_num, &assignment->n);
    assignment->task_names = create_task_array_names(assignment->task_num);
    assignment->resources_names = create_task_array_names(assignment->resources_num);
}

void fill_names (Assignment* assignment) {

    clear_buffer();

    get_name(assignment->task_names, assignment->task_num, "tasks");
    printf("\n");
    get_name(assignment->resources_names, assignment->resources_num, "resources");
}

void fill_cell(char *task_name, char *resources_name, int *cell) {
    printf("Enter the cost for task %s from resource %s: \n", task_name, resources_name);
    scanf("%d", cell);
};

void fill_matrix(Assignment *assignment) {
    int n = assignment->n;

    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            if (i < assignment->resources_num && j < assignment->task_num) {
                fill_cell(assignment->task_names[j], assignment->resources_names[i], &assignment->costs[i][j]);
            }
            else {
                assignment->costs[i][j] = 0;
            }
        }

    }
}


void print_header(Assignment* assignment) {
    printf("%-25s", "");
    for (int i=0; i<assignment->task_num; i++) {
        if (i < assignment->task_num) {
            printf("%-25s", assignment->task_names[i]);
        }
        else {
            printf("%-25s", "");
        }
    }
}

void print_matrix(Assignment* assignment) {
    print_header(assignment);
    printf("\n");

    for (int i=0; i<assignment->resources_num; i++) {
        if (i < assignment->resources_num) {
            printf("%-25s", assignment->resources_names[i]);
        }
        else {
            printf("%-25s", "");
        }

        for (int j=0; j<assignment->n; j++) {
            printf("%-25d", assignment->costs[i][j]);
        }
        printf("\n");
    }
}


void find_min_row(int *row, int n) {
    int min = row[0];
    for (int i=0; i<n; i++) {
        if (min > row[i]) {
            min = row[i];
        }
    }
    for (int j=0; j<n; j++) {
        row[j] = row[j] - min;
    }
}

void step_1(Assignment* assignment) {
    for (int i=0; i<assignment->n; i++) {
        find_min_row(assignment->costs[i], assignment->n);
    }
}

void find_min_col(Assignment *assignment, int col_index) {
    int n = assignment->n;
    int min = assignment->costs[0][col_index];

    for (int i=0; i<n; i++) {
        if (min > assignment->costs[i][col_index]) {
            min = assignment->costs[i][col_index];
        }
    }

    for (int j=0; j<n; j++) {
        assignment->costs[j][col_index] -= min;
    }
}

void step_2(Assignment* assignment) {
    for (int i=0; i<assignment->n; i++) {
        find_min_col(assignment, i);
    }
}


void find_zeros(Assignment *assignment, int zeros[][20]) {
    for (int i=0; i<assignment->n; i++) {
        for (int j=0; j<assignment->n; j++) {
            if (assignment->costs[i][j] == 0) {
                zeros[i][j] = 1;
            }
        }
    }
}

//building a adjecency graph list
void graph(Assignment *assignment, int zeros[][20], int g[][20]) {
    //this adjency list is created from the zero values from step_1 and step_2
    //the rows are to the left and columns are to the right
    //stores the indices of all resources (columns) that contain a zero in the current cost matrix.
    for (int i=0; i<assignment->n; i++) {
        int j=0;
        for (int l=0; l<assignment->n; l++) {
            if (zeros[i][j] == 1) {
                g[i][j++] = l;
            }
        }
        g[i][j] = -1; //mark end of graph
    }
}

//step 3

int try_kuhn(int p, int visited[], int g[][20], int m[]) {
    //m contains (number of row number of which the tasks have been assigned a resource) which task contains which column
    //g is the adjency list we made where the rows (tasks) are to the left and the columns (resources are to the right)
    //we use g to find which resources a task can be assigned to when building the maximum matching (array m).

    //if we have visited this row, we must leave
    if (visited[p]) {
        return 0;
    }
    //else mark as visited
    visited[p] = 1;

    //check if the row with the zero is connected to another column
    //for every column in row p is free
    for (int i=0; g[p][i] != -1; i++) {
        int l = g[p][i]; //catch the free column and leave

        //if the column is not free, try to find another place we can find a free column a.k.a is there a way i can move the occupied column so i can free it?
        if (m[l] == -1 || try_kuhn(m[l],visited, g, m)) {
            m[l] = p;
            return 1;
        }
    }
    return 0;

}

int step_3(Assignment* assignment, int g[][20], int m[]) {
    //these are used to initialise these arrays and calling try_kuhn inside it
    memset(m, 0, 20* sizeof(int));
    int visited[20];
    int matched = 0;

    for (int i=0; i<assignment->n; i++) {
        memset(visited, 0, 20 * sizeof(int));
        try_kuhn(i, visited, g, m);
        matched++;
    }
    return matched;
}

//Find the minimum number of lines needed to cover all zeros in the matrix.

void marking_lines(Assignment *assignment, int g[][20], int m[], int mark_row[], int mark_col[]) {

    //marking all as unmatched rows - they didn't get a zero assigned - which rows are currently not matched?
    for (int p=0; p<assignment->n; p++) {
        int matched =0; //intially marking all as unmatched

        for (int l=0; l < assignment->n; l++) {
            if (m[l] = p) {
                matched = 1;
                break;
            }
        }
        if (!matched) {
            mark_row[p] = 1;
        }
    }

    //look through entire matrix/graph and keep spreading marking there is no new marks appear
    int changed = 1;
    while (changed) {
        changed = 0;

        //for each marked row look for the connected row in zero cost
        for (int i=0; i<assignment->n; i++) {
            //skip if it's not marked
            if (!mark_row[i]) {
                continue;
            }
            //for each marked the column
            for (int j=0; g[i][j] != -1; j++) {
                int l = g[i][j];
                if (!mark_col[l]) {
                    mark_col[l] = 1;
                    changed = 1;
                }
            }
        }

        //now from the column mark the row it's connected to
        //if the column has a matched row mark the row
        //the column we want to occupy, who owns it?

        for (int k=0; k<assignment->n; k++) {
            if (!mark_col[k]) {
                continue;
            }
            if (m[k] != -1 && !mark_row[m[k]]) {
                mark_row[m[k]] = 1;
                changed = 1;
            }
        }


    }


}

//cover ever unmarked row and every marked column
void covering(int n, int mark_row[], int mark_col[], int covered_row[], int covered_col[]) {
    for (int i=0; i<n; i++) {
        covered_row[i] = !mark_row[i];  //looking if the row is not marked then it is marked by the column thus it is covered
        covered_col[i] = mark_col[i];  //if the column is marked then it is part of columns covered
    }

}

//step 4

void step_4(Assignment* assignment, int mark_row[], int mark_col[]) {

    //find the smallest uncovered value - so we are looking through uncovered rows and columns
    int min_val = INT_MAX;  //using max value possible for object of int type from limits library - (2^31 - 1)
    for (int i=0; i<assignment->n; i++) {
        if (!mark_row[i]) {
            continue;
        }
            for (int j=0; j<assignment->n; j++) {
                if (mark_col[j]) {
                    continue;
                }
                if (assignment->costs[i][j] < min_val) {
                    min_val = assignment->costs[i][j];
                }
            }
    }


    //now subtract min uncovered value from all uncovered values and add it to the values covered by two intersecting lines
    //shifting values to get more zero assignment - bringing down the cost itself
    for (int k=0; k < assignment->n; k++) {
        for (int l=0; l < assignment->n; l++) {
            if (!mark_row[k] && !mark_col[l]) {
                assignment->costs[k][l] -= min_val;  //want to create new zeros
            }
            if (mark_row[k] && mark_col[l]) {
                assignment->costs[k][l] += min_val;  //this cell had the value subtracted twice so we must add to it
            }
        }
    }
}


//step 5
//note: the m array stores the row number of the tasks that have been assigned a resource
void step_5(Assignment* assignment, int m[], int original_cost[][20]) {
    int total_cost = 0;

    for (int i=0; i<assignment->n; i++) {
        //if the resource i has been assigned a task? -1 means that it has not been assigned a task
        if (m[i] != -1) {
            int task = m[i]; //we get the row index of the assigned task
            printf("%s -> %s cost: %d", assignment->task_names[task], assignment->resources_names[i], original_cost[task][i]);
            total_cost += original_cost[task][i];
        }
    }
    printf("Total cost: %d", total_cost);
}

