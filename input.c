#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TERMS 8
int main() {
    char inputs[8][20];
    char * terms[20];

    int i, j;
    int num_terms = 0;
    printf("Enter terms: (up to 8):\n");
    for (i = 0; i < 8; i++) {
        if (fgets(inputs[i], 100, stdin) == NULL) {
            break;
        }
        //inputs[i][strcspn(inputs[i], "\n")] = '\0';
    }
    char* token = strtok(inputs[i], " ");
    while (token != NULL) {
        if(token[0] == '"' && token[strlen(token) -1] == '"') {
            terms[num_terms] = token;
            num_terms++;
        }
        else {
            char *token2 = strtok(token, " ");
            while (token2 != NULL) {
                terms[num_terms] = token2;
                num_terms++;
                terms[num_terms] = strtok(NULL, " ");
            }
        }
        token = strtok(NULL, " ");
    }
    for (j = 0; j < num_terms; j++) {
        printf("%s ", terms[j]);
    }

    return 0;
}