
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ADTList.h"

// Initialize the history structure
void init_history(void);

// Add a command to the history
void add_history(char *line);

// Check for history commands
char *check_history(char *line);

// Present the history of the commands
void show_history(void);

// Retrack the history command in the n-th place
char *history_n(char *line);

// Destoying the history structure
void destruct_history(void);