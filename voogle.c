#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

FILE * fp_niv ;
char * stringLower(char * str); 
int checker_for_token (char * term, char * string);
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
	const char space[2] = " ";
	char *token = strtok(string, space);
	string = remove_book_verse(string);
	int term_length = strlen(term);
	int result, result_lower;
	while (token != NULL) {
		if((strcmp(token, term)) != 0) {
			result = strncmp(token, term, term_length);
			if(result == 0) {
				return 0;
			}
		}
		else if ((strcmp(stringLower(token), stringLower(term))) != 0) {
			result_lower = strncmp(token, term, term_length);
			if(result_lower == 0) {
				return 0;
			}
		}
		token = strtok(NULL, space);
	}
	return 1;
}

int checker_for_token_star (char * term, char * string) {
	const char space[2] = " ";
	char * new_string = strdup(string);
	new_string = remove_book_verse(new_string);
	char * token = strtok(new_string, space);
	int term_length = strlen(term);
	int result, result_lower;
	while (token != NULL) {

		if((strcmp(token, term)) != 0) {
			result = strncmp(token, term, term_length);
			if(result == 0) {
				free(new_string);
				return 0;
			}
		}
		char * low_token = stringLower(token);
		char * low_term = stringLower(term);
		if ((strcmp(low_token, low_term)) != 0) {
			result_lower = strncmp(low_token, low_term, term_length);
			if(result_lower == 0) {
				free(new_string);
				return 0;
			}
		}
		token = strtok(NULL, space);
	}
	free(new_string);
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

	// char * s = 0x0 ;
	// char z = 'Z';
	// char * jude = "Jude";
	// while ((s = read_a_line())) {
	// 	if (strstr(s, jude) != NULL) {
	// 		printf("%s\n", s) ;
	// 		free(s) ;
	// 	}
	// }
	char * jude = "Jude";
	char * s = "Mat 2:1 After Jesus was born in Bethlehem in Judea, during the time of King Herod, Magi from the east came to Jerusalem";
	char * new_s = remove_book_verse(s);
	int x,y,z;
	x = checker_for_token_star(jude,new_s);
	printf("%d\n", x);
	printf("%s\n", new_s);
	free(new_s) ;
		
	fclose(fp_niv) ;
	return 0;
}
	



