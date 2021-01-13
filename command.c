/**
 * Lab 2 - Program Pointers
 *
 * Miguel Oyler-Castrillo & Vincent Dong
 *
 * command.c - Source file with your parsing library implementation.
 *
 */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "command.h"

//appends char c to the end of char pointer str and moves the special null character to the right by one word
//
//Parameters:
//
//-str: the char pointer / char array we want to append to
//-c : the character we want to append
void append(char* str, char c){
	while(*str){
		*str++;
	}
	*str = c;
	*str++ == '\0';
}

/**
 * command_parse:
 *
 * Parse a command-line string, report foreground/background status,
 * and return a command array.  Each element in the command array
 * corresponds to one word in the command string, in order.
 *
 * A command with '&' after the last word is a background command.
 * The '&' is not part of the resulting command array.  Repetitions or
 * other placements of '&' constitute invalid command lines.
 *
 * Parameters:
 *
 * - line: non-NULL pointer to a command-line string.
 *
 * - foreground: non-NULL pointer to an int where foreground (1)/
 *   background (0) status of the command line should be stored.
 * 
 * Return:
 *
 * - a pointer to a NULL-terminated array of strings (char**) if given
 *   a valid command-line string.
 *
 * - NULL if given a command-line string with invalid placement of '&'
 */
char** command_parse(char* line, int* foreground) {
  // Check arguments: must be non-NULL pointers.
  assert(line);
  assert(foreground);
  int numWords = 0;
  int arIndex = 0;
  int wordLen = 0; // recording the length of line because it'll come in handy later when allocating space for strings
  char* line2 = line; //creating a copy because i'm going to want to loop thru line again later
 while (*line) { //loop thru string
	if(*line == '&'){ //check if there is a special '&' character
		char* and = line; //initiate a char pointer starting from wherever we spotted '&'
		while(*and++){ //loop thru the rest of the string starting AFTER first occurence of '&'
			if (*and == ' '){
				continue;
			}
			if(*and){ //the only thing that should follow '&' is spaces and null pointer
				return NULL; //if whatever came after '&' was NOT a space or a null, then terminate
			}
		}
		*foreground = 0; //turns out the '&' was legit so this is background therefore foreground is false
	}	
	if(*line == ' '){ //check if whitespace
	       *line++; //increment pointer and continue with loop
	       wordLen++;
	       continue;
	}
	if (*(line + 1) == ' ' || !(*(line + 1)) || *(line + 1) == '&') { //checks to see if this is the end of a word
		numWords++; //increment numwords because it was a word
	}
	wordLen++;
	*line++; //there is more to this word... continue loop
	}
  //get the number of words in the string
  char** strAr= malloc((numWords + 1) * sizeof(char*)); //allocate the space for our char pointers
  while(*line2){ //loop through string one more
	  if(arIndex <= numWords){
	  	*(strAr + arIndex) = malloc(wordLen * sizeof(char)); //allocate the space for each o the words
	  }
	  if (*line2 == '&'){
		  break;
	  }
	  while(*line2 == ' '){ //skip whitespace, for some reason *line2 != ' ' just doesn't work
		  if(*line2){
			*line2++;
		  }
	  }
	  while (*(line2) != ' ' && *line2 != '&'){ //loop until the end of a word
		 if (*line2 == ' ') {//again, for some reason *line2 != ' ' does't work
			 *line2++;
			continue;
		 } 
		  append(*(strAr + arIndex), *line2); //add a new character to the word im looping thru
		  *line2++;
	  	  }
	if (arIndex < numWords){
		arIndex++;
	}
	if (arIndex == numWords){
		*(strAr + numWords) = NULL; //adding a NULL to mark the end of the command array
	}	
	if (*line2){
        	*line2++;
	}
	
  }
  return strAr;
}

/**`
 * command_show:
 *
 * Print the structure of a command array.
 *
 * Parameters:
 *
 * - command: a non-NULL pointer to the command array to print.
 */
void command_show(char** command) {
  // Check argument: must be non-NULL pointer.
  assert(command);

  // IMPLEMENT ME
  for (char** p = command; *p; p++){
	printf("(%s)\n", *p);
  }
  printf("(NULL) \n");

}

/**
 * command_print:
 *
 * Print a command array in the form of a command-line string.
 * You should *not* include a newline at the end.
 *
 * Parameters:
 *
 * - command: non-NULL pointer to the command array to print.
 */
void command_print(char** command) {
  // Check argument: must be non-NULL pointer.
  assert(command);

  // IMPLEMENT ME
  char**p = command;
  printf("[");
  while (*p) {
	char*c = *p;
	while (*c) {
		printf("%c", *c);
		*c++;
	}
	if (*(p + 1)){
		printf(", ");
	}
	*p++;
  }
  printf("]");
}

/**
 * command_free:
 *
 * Free all parts of a command array created by command_parse.
 *
 * Parameters:
 *
 * - command: non-NULL pointer to the command array to free.
 */
void command_free(char** command) {
  // Check argument: must be non-NULL pointer.
  assert(command);

  // IMPLEMENT ME
  int arrLen = 0;
  int arrIndex = 0;
  for(char** p = command; *p; p++){
	arrLen++;
	free(*p);
  }
  while(arrIndex <= arrLen){
	  free(*(command + arrIndex));
	  arrIndex++;
  }
  free(command);
}

