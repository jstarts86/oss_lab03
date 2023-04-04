#include <ctype.h>
#include <string.h>
#include <stdio.h>

char * stringLower(char * str) {

	int len = strlen(str);
    for (int i = 0; i < len; i++) {
        str[i] = tolower(str[i]);
    }
	return str;
}

int main() {
    char str[] = "Hello World!";
    printf("%s\n", stringLower(str)); // prints "hello world!"
    return 0;
}