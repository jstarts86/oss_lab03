#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUTS 8
#define MAX_TERM_LEN 50

int main() {
    char input[MAX_INPUTS][MAX_TERM_LEN];
    char *term;
    int num_inputs = 0;
    
    printf("Enter up to %d string inputs, separated by spaces:\n", MAX_INPUTS);
    
    // read input and split into terms
    char line[MAX_INPUTS * MAX_TERM_LEN];
    fgets(line, sizeof(line), stdin);
    term = strtok(line, " ");
    while (term != NULL && num_inputs < MAX_INPUTS) {
        if (term[0] == '\"' && term[strlen(term)-1] == '\"') { // check if term is surrounded by quotes
            strncpy(input[num_inputs], term+1, strlen(term)-2); // copy term without quotes
            input[num_inputs][strlen(term)-2] = '\0'; // ensure null-terminated
        } else {
            strncpy(input[num_inputs], term, MAX_TERM_LEN); // copy term
            input[num_inputs][MAX_TERM_LEN - 1] = '\0'; // ensure null-terminated
        }
        num_inputs++;
        term = strtok(NULL, " ");
    }
    
    // print out the inputs
    printf("You entered %d inputs:\n", num_inputs);
    for (int i = 0; i < num_inputs; i++) {
        printf("%s\n", input[i]);
    }
    
    return 0;
}
