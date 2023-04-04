#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char input[8][100];
    char *terms[100];
    int i, j, num_terms;

    // prompt the user to enter up to 8 string inputs
    printf("Enter up to 8 string inputs (press Ctrl-D to end):\n");
    for (i = 0; i < 8; i++) {
        printf("Input %d: ", i + 1);
        if (fgets(input[i], 100, stdin) == NULL) {
            // stop asking for inputs if the user enters Ctrl-D
            break;
        }
        // remove trailing newline character from input
        input[i][strcspn(input[i], "\n")] = '\0';
    }

    // split each input into separate terms and output them
    for (i = 0; i < 8; i++) {
        if (strlen(input[i]) == 0) {
            break;
        }
        num_terms = 0;
        terms[num_terms] = strtok(input[i], " ");
        while (terms[num_terms] != NULL) {
            num_terms++;
            terms[num_terms] = strtok(NULL, " ");
        }
        printf("Input %d: ", i + 1);
        for (j = 0; j < num_terms; j++) {
            printf("%s ", terms[j]);
        }
        printf("\n");
    }

    return 0;
}