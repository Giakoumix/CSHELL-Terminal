
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fnmatch.h>
#include <glob.h>

#include "ADTVector.h"

#define MAX_PIPECMD 500
#define MAX_SINGLCMD 150
#define MAX_ARGUMENT 50

#define MAX_PIPES 5 

struct cmd *parse_back(char *line);

// Parses a single command consisting only 
// from redirection and executable commands
struct cmd *parse_singlecmd(char *line);

// Can parse a pipe command
// If there are no pipes it calls
// parse_singlecmd
struct cmd *parse_pipe(char *line);

// Parses an exec command
struct exec_cmd *parse_exec(char *line);

// Destructs the command structure
void destruct_command(struct cmd *command);

