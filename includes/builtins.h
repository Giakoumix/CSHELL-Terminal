
// All the built-in shell commands
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum BUILTINS {
    CD,
    PWD
};

// Checks if a command is a built-in
// and calls the corresponding function
int check_builtin(char *line);

// Changes the current directory
void change_dir(char *line);

// Prints the working directory
void print_working_dir(char *line);