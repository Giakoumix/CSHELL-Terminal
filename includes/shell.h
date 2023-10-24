
#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#include "commands.h"
#include "parser.h"

enum EXIT_STATUS {
    ERROR,
    NORMAL,
    BACKGRND
};

// Takes a Tree-like command data structure
// and executes its instructions
int execute(struct cmd *cmd);
