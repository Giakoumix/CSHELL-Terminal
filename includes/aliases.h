
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ADTMap.h"

// Initializes the alias data structure
void init_alias(void);

// Checks for alias command
int check_alias(char *line);

// Creating an alias
void create_alias(char *line);

// Checking for matching strings
// in the command
char *replace_alias(char *line);

// Destroying an alias
void destroy_alias(char *line);

// Destroying the structure of alias
void destruct_alias();

