#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 100
#define MAX_NUM_INPUT 8

FILE * fp_niv ;
char * read_a_line ();
char * stringLower(char * str); 
int checker_for_token (char * term, char * string);
int checker_for_token_star (char * term, char * string);
int checker_for_token_dash (char * term, char * string);
char * remove_book_verse(char * str);
int tokenizer(char *input, char **terms);
char* remove_last_char(const char* str);
char* remove_first_char(const char* str);
char * read_a_line ()
{
	static char buf[BUFSIZ] ;
	static int buf_n = 0 ;
	static int curr = 0 ;

	if (feof(fp_niv) && curr == buf_n - 1)
		return 0x0 ;

	char * s = 0x0 ;
	size_t s_len = 0 ;
	do {
		int end = curr ;
		while (!(end >= buf_n || !iscntrl(buf[end]))) {
			end++ ;
		}
		if (curr < end && s != 0x0) {
			curr = end ;
			break ;
		}
		curr = end ;
		while (!(end >= buf_n || iscntrl(buf[end]))) {
			end++ ;
		}
		if (curr < end) {
			if (s == 0x0) {
				s = strndup(buf + curr, end - curr) ;
				s_len = end - curr ;
			}
			else {
				s = realloc(s, s_len + end - curr + 1) ;
				s = strncat(s, buf + curr, end - curr) ;
				s_len = s_len + end - curr ;
			}
		}
		if (end < buf_n) {
			curr = end + 1 ;
			break ;
		}

		buf_n = fread(buf, 1, sizeof(buf), fp_niv) ;
		curr = 0 ;
	} while (buf_n > 0) ;
	return s ;
}


int tokenizer(char *input, char **terms) {
    int i = 0;
    int quotes = 0;
    char *token_start = input;

    strtok(input, "\n");

    while (*input != '\0') {
        if (*input == '\"') {
            quotes = !quotes;
        }

        if (*input == ' ' && !quotes) {
            *input = '\0';
            terms[i++] = token_start;
            token_start = input + 1;
        }

        input++;
    }

    // handle last token
    if (i < MAX_NUM_INPUT && token_start != input) {
        terms[i++] = token_start;
    }

    if (i > MAX_NUM_INPUT) {
        printf("Error: Input contains more than %d tokens.\n", MAX_NUM_INPUT);
        return -1;
    }

    return i;
}


//iterates through a string making all letters lower case using tolower
char * stringLower(char * str) {

	int len = strlen(str);
    for (int i = 0; i < len; i++) {
        str[i] = tolower(str[i]);
    }
	return str;
}

// (i.e., an alphanumeric string without whitespace): a verse satisfies 
//this condition iff a token in the verse matches with the given token in case- insensitive way
int checker_for_token (char * term, char * string) {
	char * new_string = remove_book_verse(string);
	const char space[2] = " ";
	char * token = strtok(new_string, space);
	char * replacement = strdup(term);
	char * lowerTerm = stringLower(replacement);
	int term_length = strlen(term);
	
	while (token != NULL) {
		char * lowerToken = stringLower(token);

		int strcmp_lower_result = strcmp(lowerToken, lowerTerm);
		if((strcmp(token, term)) == 0) {
			return 0;
		}
		if(strcmp_lower_result == 0) {
			return 0;
		}
		token = strtok(NULL, space);
	}
	return 1;
}
/*
	 a verse satisfies this condition iff there is a token in the
	  verse whose prefix matches with token in case-insensitive way
*/
int checker_for_token_star (char * term, char * string) {
	
	char * new_string = remove_book_verse(string);
	const char space[2] = " ";
	char * token = strtok(new_string, space);
	char * replacement = strdup(term);
	char * lowerTerm = stringLower(replacement);
	int term_length = strlen(term);
	if(term[term_length-1] == '*') {
		term = remove_last_char(term);
		term_length = strlen(term);
		while (token != NULL) {
			int result1 = strncmp(token, term, term_length);
			char * lowerToken = stringLower(token);
			int strcmp_lower_result = strcmp(lowerToken, lowerTerm);
			int resultLower = strncmp(lowerToken, lowerTerm, term_length);
			if((strcmp(token, term)) != 0) {
				if(result1 == 0) {
					return 0;
				}
			}
			if (strcmp_lower_result != 0) {
				if(resultLower == 0) {
					return 0;
				}
			}
			token = strtok(NULL, space);
		}
		return 1;
	} 
	else {
		return 2;
	}
	
}
/*
	 a verse satisfies this condition iff the verse has 
	 no token that matches with token in case-insensitive way
*/
int checker_for_token_dash (char * term, char * string) {
	char * new_string = remove_book_verse(string);
	const char space[2] = " ";
	char * token = strtok(new_string, space);
	char * replacement = strdup(term);
	char * lowerTerm = stringLower(replacement);
	int term_length = strlen(term);
	if(term[0] == '-') {
		while (token != NULL) {
			term = remove_first_char(term);
			term_length = strlen(term);
			char * lowerToken = stringLower(token);
			term_length = strlen(term);
			int strcmp_lower_result = strcmp(lowerToken, lowerTerm);
			if((strcmp(token, term)) == 0) {
				return 1;
			}
			if(strcmp_lower_result == 0) {
				return 1;
			}
			token = strtok(NULL, space);
		}
		return 0;
	} else {
		return 2;
	}
	

}
// removes the book and verse from the line
char * remove_book_verse(char * str) {
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

char* remove_last_char(const char* str) {
    int len = strlen(str);
    if (len == 0) {
        return NULL;
    }
    char* new_str = (char*) malloc(len);
    strncpy(new_str, str, len - 1);
    new_str[len - 1] = '\0';
    return new_str;
}

char* remove_first_char(const char* str) {
	int len = strlen(str);

	if(len == 0) {
		return NULL;
	}
	char* new_str = (char*) malloc(len);
	memmove(new_str, str +1, len);
	new_str[len - 1] = '\0';
	return new_str;
}
int main (int argc, char ** argv)
{
	// char input[MAX_INPUT_LENGTH];
    // char *tokens[MAX_NUM_INPUT];

    // printf("Enter up to 8 tokens: ");
    // fgets(input, 100, stdin);

    // int num_tokens = tokenizer(input, tokens);

    // if(num_tokens < 0) {
    //     return 1;
    // }

    // printf("Tokens: ");
    // for(int j = 0; j < num_tokens; j++) {
    //     printf("%s\n", tokens[j]);
    // }
    // printf("\n");


	//fp_niv = fopen("NIV.txt", "r") ;

	char * jude = "-Jude" ;
	char * s = "Jude 2:1 After Jesus was born in Bethlehem in Jude, during the time of King Herod, Magi from the east came to Jerusalem";
	char * new_s = remove_book_verse(s);
	int x,y,z;
	//x = checker_for_token_star(jude,s);
	//printf("Checker for * %d\n", x);
	// y = checker_for_token(jude,s);
	// printf("Checker for normal token %d\n", y);
	z = checker_for_token_dash(jude,s);
	printf("Checker for dash token(1 if it token is in line/ 0 if it is not):%d\n", z);
	printf("%s\n", s);
	//free(s);
	//fclose(fp_niv) ;

	
	return 0;
}
	



