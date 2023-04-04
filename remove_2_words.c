#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* remove_first_two_words(char* str) {
    int space_count = 0;
    char *ptr = str;
    while (*ptr != '\0') {
        if (*ptr == ' ') {
            space_count++;
            if (space_count == 2) {
                char *new_str = (char*)malloc(strlen(ptr));
                strcpy(new_str, ptr+1);
                return new_str;
            }
        }
        ptr++;
    }
    char *new_str = (char*)malloc(strlen(str));
    strcpy(new_str, str);
    return new_str;
}

int main() {
    char str[100];
    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);

    char *result = remove_first_two_words(str);

    printf("After removing the first two words: %s\n", result);

    free(result);

    return 0;
}
