#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#define NUM_TASKS 20
#define NUM_RESOURCES 20
#define MAX_NAME_LENGTH 100



int main(void) {

    Assignment a;
    intialize_assignment(&a);
    fill_names(&a);
    fill_matrix(&a);
    print_matrix(&a);

    printf("\n");

    //fill in a copy of the cost matrix
    int original_cost[NUM_TASKS][NUM_RESOURCES];
    for (int i=0; i<NUM_TASKS; i++) {
        for (int j=0; j<NUM_RESOURCES; j++) {
            original_cost[i][j] = a.costs[i][j];
        }
    }

    int m[20];
    int mark_rows[20];
    int mark_cols[20];
    int covered_rows[20];
    int covered_cols[20];
    int g[NUM_TASKS][NUM_RESOURCES];
    int matched = 0;


    step_1(&a);
    print_matrix(&a);
    printf("\n");
    step_2(&a);
    print_matrix(&a);

    int zeros[NUM_TASKS][NUM_RESOURCES];

    while (matched < a.n) {
        for (int i=0; i<NUM_TASKS; i++) {
            for (int j=0; j<NUM_RESOURCES; j++) {
                zeros[i][j] = 0;
            }
        }

        find_zeros(&a, zeros);
        graph(&a, zeros, g);

        matched = step_3(&a, g, m);

        if (matched < a.n) {
            marking_lines(&a, g, m, mark_rows, mark_cols);
            covering(a.n, mark_rows, mark_cols, covered_rows, covered_cols);
            step_4(&a, covered_rows, covered_cols);

        }

    }

    step_5(&a, m, original_cost);



}
