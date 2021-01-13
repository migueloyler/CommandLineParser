# Makefile to compile the various parts of the lab.

#### Variables ####

# C compiler
CC = gcc

# Flags to pass to C compiler
CFLAGS = -Wall -g --std=c99

##### Targets ####
all: command_test

# Command parser
command.o: command.c command.h
	$(CC) $(CFLAGS) -c $<

# Test harness
command_test: command_test.c command.o
	$(CC) $(CFLAGS) -o $@ $< command.o

# Clean up executables
clean:
	rm -f *.o command_test
	rm -rf *.dSYM
