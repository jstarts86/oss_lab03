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

char * checker_for_token_star () {
	fp_niv = fopen("NIV.txt", "r") ;
	char * output = 0x0 ;
	char * s = 0x0 ;
	char * token = "Jude";
	while ((s = read_a_line())) {
		char * b_verse = strstr(s, " ");
		char * verse = strstr(b_verse, " ");
		if (strstr(b_verse, token) != NULL) {
			output = s;

		}
	fclose(fp_niv) ;
	}
	return output;
}

void remove_book_verse(char * line, char * final_str) {
    char *first_space = strchr(line, ' ');
    char buffer_str[1000];


    if (first_space != NULL) {
        strcpy(buffer_str, first_space + 1);
    }  
    char *final_space = strchr(buffer_str, ' ');

    if (final_space != NULL) {
        strcpy(final_str, final_space + 1);
    } 
   
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
	char * mod_s = 0x0;
	while ((s = read_a_line())) {
		char * b_verse = strchr(s, ' ');
		char * verse = strchr(b_verse, ' ');
		if (strstr(b_verse, token) != NULL) {
			remove_book_verse(s,mod_s);
			printf("%s\n", mod_s) ;
			free(s) ;
		}
	
	}
	fclose(fp_niv) ;
	return 0;
}


