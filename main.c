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

char ** find(char * token_include, char * token_exclude, char * string, char * input_book, int input_chap)
{
	// declaration and initialization variables
	char verse[BUFSIZ] = "";								
	char * book_name = (char *)malloc(sizeof(char) * 4);
	char * chap_num = (char *)malloc(sizeof(char) * 4);
	char * verse_num = (char *)malloc(sizeof(char) * 4);
	char * detail = (char *)malloc(sizeof(char) * BUFSIZ);
	int chap = 0;
	int count = 0;

	// open file
	fp_niv = fopen("NIV.txt", "r");

	// return variables
	char ** results = malloc(sizeof(char *) * BUFSIZ);		// contain strings
	for(int i = 0; i < BUFSIZ; i++)
		results[i] = malloc(sizeof(char) * BUFSIZ);
	
	// modify string
	while(fgets(verse, BUFSIZ, fp_niv) != NULL){
		strcpy(results[count], verse);

		book_name = strtok(verse, " ");
		chap_num = strtok(NULL, ":");
		verse_num = strtok(NULL, " ");
		detail = strtok(NULL, "\n");

		// book:code
		if(strcmp(book_name, input_book) || input_book == NULL)
			continue;

		// chapter:num
		chap = atoi(chap_num);
		if(input_chap == 0x0 || chap != input_chap)
			continue;

		// "string"
		if(string == 0x0 || strstr(detail, string) == NULL)
			continue;

		// for token(case-insensitive way)
		for(int i = 0; detail[i] != 0; i++)
			detail[i] = tolower(detail[i]);

		// token, token*
		if(token_include == 0x0 || strstr(detail, token_include) == NULL)
			continue;

		// -token
		if(token_exclude == 0x0 || strstr(detail, token_exclude) != NULL)
			continue;

		count++;
	}

	// close file
	fclose(fp_niv);

	return results;
}

int main (int argc, char ** argv)
{
	char * token_include = (char *)malloc(sizeof(char) * 64);
	char * token_exclude = (char *)malloc(sizeof(char) * 64);
	char * string = (char *)malloc(sizeof(char) * BUFSIZ);
	char * book = (char *)malloc(sizeof(char) * 16);
	char * chap = (char *)malloc(sizeof(char) * 16);

	fp_niv = fopen("NIV.txt", "r") ;

	// test code of read_a_line
	/*
	char * s = 0x0 ;

	while ((s = read_a_line())) {
		printf("%s\n", s) ;
		free(s) ;
	}
	*/

	char * user_input;
	scanf("%s", user_input);

	token_include = "";
	token_exclude = strchr(user_input, '-');
	string = strchr(user_input, '\"');
	book = strstr(user_input, "book:");
	chap = strstr(user_input, "chapter:");

	char ** result = find(token_include, token_exclude, string, book, chap);

	fprintf(stdout, "%s", result);

	fclose(fp_niv) ;
	free(result);
}