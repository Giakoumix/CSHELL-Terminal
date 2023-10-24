
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXARGS 6
#define BACKCMD 5

#define INPUT 0
#define OUTPUT 1

#define READ 0
#define WRITE 1

enum CMD_TYPE {
    EXEC,
    REDIR,
    PIPE,
    BACK
};

struct cmd {
    enum CMD_TYPE type;
};

struct exec_cmd {
    enum CMD_TYPE type;
    char **argv;
    int size;
    // char *argv[MAXARGS+1];
};

struct redir_cmd {
    enum CMD_TYPE type;
    char *file_in;
    char *file_out;
    struct exec_cmd *cmd;
};

struct pipe_cmd {
    enum CMD_TYPE type;
    struct cmd *left;
    struct cmd *right;
};

struct back_cmd {
    enum CMD_TYPE type;
    struct cmd *list[BACKCMD];
    int how_many;
};

struct exec_cmd *exec_cmd(void);
struct redir_cmd *redir_cmd(struct exec_cmd *exec, char *file_in, int size_in,char *file_out, int size_out);
struct pipe_cmd *pipe_cmd(struct cmd *left, struct cmd *right);
struct back_cmd *back_cmd(void);