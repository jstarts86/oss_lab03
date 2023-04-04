#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

FILE * fp_niv ;

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

char * checker_for_token_star (char * token, char * string) {
	if(string = 0)
	while ((s = read_a_line())) {
		if (strstr(s, token) != NULL) {
			output = s;
		}
	}
	return output;
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
	char * s = 0x0 ;
	char * token = "Jude";
	while ((s = read_a_line())) {
		if (strstr(s, token) != NULL) {
			char * mod_s = remove_book_verse(s);
			printf("%s\n", mod_s) ;
			free(mod_s) ;
		}
	
	}
	fclose(fp_niv) ;
	return 0;
}


