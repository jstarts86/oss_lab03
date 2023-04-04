#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

FILE * fp_niv ;
char * stringLower(char * str); int checker_for_token (char * term, char * string);
int checker_for_token_star (char * term, char * string);
char * remove_book_verse(char * str);
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

char * stringLower(char * str) {

	int len = strlen(str);
    for (int i = 0; i < len; i++) {
        str[i] = tolower(str[i]);
    }
	return str;
} 

int checker_for_token (char * term, char * string) {
	char * new_string = remove_book_verse(string);
	const char space[2] = " ";
	char * token = strtok(new_string, space);
	char * replacement = strdup(term);
	char * lowerTerm = stringLower(replacement);
	int term_length = strlen(term);
	
	while (token != NULL) {
		int result1 = strncmp(token, term, term_length);
		char * lowerToken = stringLower(token);

		int strcmp_lower_result = strcmp(lowerToken, lowerTerm);
		if(((strcmp(token, term)) || strcmp_lower_result) == 0) {
			return 0;
		}
		token = strtok(NULL, space);
	}
	return 1;
}

int checker_for_token_star (char * term, char * string) {
	char * new_string = remove_book_verse(string);
	const char space[2] = " ";
	char * token = strtok(new_string, space);
	char * replacement = strdup(term);
	char * lowerTerm = stringLower(replacement);
	int term_length = strlen(term);
	
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



int main (int argc, char ** argv)
{

	fp_niv = fopen("NIV.txt", "r") ;

	char * jude = "Jude" ;
	char * s = "Jude 2:1 After Jesus was born in Bethlehem in Jude , during the time of King Herod, Magi from the east came to Jerusalem";
	//char * new_s = remove_book_verse(s);
	int x,y,z;
	// x = checker_for_token_star(jude,s);
	// printf("Checker for * %d\n", x);
	y = checker_for_token(jude,s);
	printf("Checker for normal token %d\n", y);
	printf("%s\n", s);
	//free(s);
		
	fclose(fp_niv) ;
	return 0;
}
	



