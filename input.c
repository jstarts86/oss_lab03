#include <stdio.h>
#include <string.h>

#define MAX_TERMS 8
#define TERM_LENGTH 20

int main() {
    char terms[MAX_TERMS][TERM_LENGTH];
    int num_terms = 0;

    // prompt the user to enter up to 8 search terms
    printf("Enter up to %d search terms (press Ctrl+D to finish):\n", MAX_TERMS);
    char term[TERM_LENGTH];
    while (num_terms < MAX_TERMS && fgets(term, TERM_LENGTH, stdin) != NULL) {
        term[strcspn(term, "\n")] = '\0'; // remove trailing newline character
        if (strlen(term) == 0) {
            break;
        }
        strcpy(terms[num_terms], term);
        num_terms++;
    }

    // print the search terms entered by the user
    printf("You entered the following search terms:\n");
    for (int i = 0; i < num_terms; i++) {
        printf("%s\n", terms[i]);
    }

    return 0;
}
