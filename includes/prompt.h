
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROMPT_SIZE 250
#define CMD_SIZE 1000

// Prints the current directory 
// of the prompt
void print_prompt(void);

// Gets line from keyboard and
// cleans it
char *get_prompt_line(void);