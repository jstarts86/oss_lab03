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
char* remove_first_char(char* str);
int check_line(char *str, char **terms, int num_terms);
int checker_for_quotes(char *term, char * string);

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
	// initializes variables
    int i = 0;
    int quotes = 0;
    char *token_start = input; 	//pointer to beginning of current token
    strtok(input, "\n"); 	//removes new line character

    while (*input != '\0') { 	//tokenizes when ever there is a space character
		// checks if current character is a quotation mark
        if (*input == '\"') {
            quotes = !quotes;
        }
		// if not it quotes and we have space, replace space with null terminator and add current token to array
        if (*input == ' ' && !quotes) {
            *input = '\0';
            terms[i++] = token_start;
            token_start = input + 1;
        }
        input++;
    }
    // add last token if there is room
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
	char * lowerTerm = stringLower(replacement); //makes the term all lower case
	int term_length = strlen(term); //gets the length of the term
	// checks to see if its a regular token that does not have a - or " at index[0], or a * at index[term_length - 1] and does not contain a :
	if(term[0] != '\"' || term[0] != '-' || term[term_length - 1] != '*' || strchr(term, ':') == NULL) { 
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
	else {
		return 2;
	}
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
		term = remove_last_char(term); // removes the * from the term
		term_length = strlen(term);
		while (token != NULL) {
			int result1 = strncmp(token, term, term_length); // compares the token with the term up to the length of the term
			char * lowerToken = stringLower(token);
			int strcmp_lower_result = strcmp(lowerToken, lowerTerm); // compares the lower case token with the lower case term up to the length of the term
			int resultLower = strncmp(lowerToken, lowerTerm, term_length);
			if((strcmp(token, term)) != 0)  { // if the token is exactly equal to the term then return false
				if(result1 == 0) { // if the token is the same to term up to term length then return true
					return 0;
				}
			}
			if (strcmp_lower_result != 0) { // if the token is exactly equal to the term then return false
				if(resultLower == 0) { // if the token is the same to term up to term length then return true
					return 0;
				}
			}
			token = strtok(NULL, space); // go to the next token after the white space
		}
		return 1; // return false if all the tokens in the string are not the prefix of term*
	} 
	else {
		return 2; // if the term is not term* then return 2
	}
	
}

/*
	 a verse satisfies this condition iff the verse has 
	 no token that matches with token in case-insensitive way
*/
int checker_for_token_dash (char * term, char * string) {
	char * new_string = remove_book_verse(string);
	const char space[2] = " ";
	char * token = strtok(new_string, space); // makes a take out of the first string before a white space character
	char * replacement = strdup(term);
	char * lowerTerm = stringLower(replacement);
	int term_length = strlen(term);
	if(term[0] == '-') { //checks if the term is term-
		term = remove_first_char(term); //get rid of - in term-
		lowerTerm = remove_first_char(lowerTerm); //get rid of - in the lowercase term-
		term_length = strlen(term); //gets the new length of term
		while (token != NULL) { //runs the loop until the token is faced with a NULL character
			char * lowerToken = stringLower(token); //makes the token lower case
			int strcmp_lower_result = strcmp(lowerToken, lowerTerm); // compares the lowercase token with the lowercase term
			if((strcmp(token, term)) == 0) { // return true if a token in a string is the same a the term
				return 1;
			}
			if(strcmp_lower_result == 0) { // return true if a lowercase token in a string is the same a the lowercase term
				return 1;
			}
			token = strtok(NULL, space); //resumes the tokenizing to the next whitespace character
		}
		return 0; // return 0 or true if no tokens are the same as the term in the line
	} else {
		return 2; //return 2 if the term is not term-
	}
	

}

// removes the book and verse from the line
// this makes it so that the function does not tokenize the chapter code and book number
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
// removes the last character and returns a string
char* remove_last_char(const char* str) {
    int len = strlen(str);
    if (len == 0) {
        return NULL;
    }
    char* new_str = (char*) malloc(len);
    strncpy(new_str, str, len - 1); // copys str without the last index into new_str
    new_str[len - 1] = '\0'; // puts a null terminating character in new_str
    return new_str;
}
// removes the first character and returns a string
char* remove_first_char(char* str) {
	int len = strlen(str);

	if(len == 0) {
		return NULL;
	}
	char* new_str = (char*) malloc(len);
	memmove(new_str, str +1, len); // moves the string one step to the left getting rid of the very first index
	new_str[len - 1] = '\0';
	return new_str;
}

int check_line(char *str, char **terms, int num_terms) { // check line return 0 if the each of the terms satisfy one of the checker function
	int count = 0;
	for(int i = 0; i < num_terms; i++) {
		if(checker_for_token(terms[i],str) == 0) {
			count++;
		}
		else if(checker_for_token_dash(terms[i],str) == 0) {
			count++;
		}
		else if(checker_for_token_star(terms[i],str) == 0) {
			count++;
		}
	}
	if(count == num_terms) {
		return 0;
	}
	else {
		return 1;
	}
}

int main (int argc, char ** argv)
{
	fp_niv = fopen("NIV.txt", "r") ;
	char * s = 0x0;
	char input[MAX_INPUT_LENGTH];
    char *tokens[MAX_NUM_INPUT];
    fgets(input, 100, stdin);

    int num_tokens = tokenizer(input, tokens);

    if(num_tokens < 0) {
        return 1;
    }
	for(int i = 0; i < num_tokens; i++) {
		while ((s = read_a_line())) {
			if((check_line(s, tokens, num_tokens)) == 0) {
				printf("%s\n", s);
				free(s);
			}
		}
    }
	fclose(fp_niv);
	return 0;
}