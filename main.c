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
    step_1(&a);
    print_matrix(&a);
    printf("\n");
    step_2(&a);
    print_matrix(&a);

    int zeros[NUM_TASKS][NUM_RESOURCES];

    for (int i=0; i<NUM_TASKS; i++) {
        for (int j=0; j<NUM_RESOURCES; j++) {
            zeros[i][j] = 0;
        }
    }



}
