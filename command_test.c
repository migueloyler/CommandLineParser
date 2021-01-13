/**
 * Write tests for your command parsing library here.
 * 
 * If you wish to call helper functions from command.c besides
 * command_parse, command_free, and command_print, you must add their
 * headers to command.h
 */
#include <stdlib.h>
#include <stdio.h>
#include "command.h"

// Sample command lines
#define NUM_COMMAND_LINES 1
static char* COMMAND_LINES[NUM_COMMAND_LINES] = {
  "I    really  hope     this  works & "
};

// Sample command arrays to test command_print and command_show.
static char* LS[4] = { "ls", "-A", "filename.txt", NULL };
static char* NANO[5] = { "nano", "command.c", "command.h", "Makefile", NULL };

// All sample command arrays
#define NUM_COMMAND_ARRAYS 2
static char** COMMAND_ARRAYS[NUM_COMMAND_ARRAYS] = {
  LS,
  NANO
};

void test_all(char* command) {
  int foreground;
  char** c;
  printf("Parsing: \"%s\"\n", command);
  c = command_parse(command, &foreground);
  if (c) {
    printf("Parsed %s command:\n", (foreground ? "foreground" : "background"));
    command_show(c);
    printf("As comand line: ");
    command_print(c);
    printf("\n");
    command_free(c);
    printf("Freed command array.\n");
  } else {
    printf("Invalid command.\n");
  }
}

/**
 * main is the top-level function that executes when the compiled
 * program is run.
*/
int main(int argc, char** argv) {

  for (int i = 0; i < NUM_COMMAND_ARRAYS; i++) {
    printf("Command array %d:\n", i);
    command_show(COMMAND_ARRAYS[i]);
    printf("As command line: ");
    command_print(COMMAND_ARRAYS[i]);
    printf("\n");
  }
  
  for (int i = 0; i < NUM_COMMAND_LINES; i++) {
    test_all(COMMAND_LINES[i]);
  }
  
  // main should technically return an int result.
  // 0 means success; nonzero means error.
  return 0;
}

