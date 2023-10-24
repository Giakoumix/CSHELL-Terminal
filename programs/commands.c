
#include "commands.h"

struct exec_cmd *exec_cmd(void) {
    struct exec_cmd *exec;

    exec = malloc(sizeof(*exec));

    exec->argv = malloc((MAXARGS+1)*sizeof(*(exec->argv)));
    for (int i = 0 ; i <= MAXARGS ; i++) {
        exec->argv[i] = malloc(50);
    }
    exec->size = MAXARGS+1;
    exec->type = EXEC;

    return  exec; 
}

struct redir_cmd *redir_cmd(struct exec_cmd *exec, char *file_in, int size_in, char *file_out, int size_out) {
    struct redir_cmd *redir;

    redir = malloc(sizeof(*redir));
    memset(redir, 0, sizeof(*redir));
    redir->file_in = malloc(size_in);
    if (file_in != NULL)
        strcpy(redir->file_in, file_in);
    else 
        redir->file_in = NULL;
    redir->file_out = malloc(size_out);
    
    if (file_out != NULL)
        strcpy(redir->file_out, file_out);
    else
        redir->file_out = NULL; 
    
    redir->cmd = exec;
    redir->type = REDIR;
    
    return redir;
}

struct pipe_cmd *pipe_cmd(struct cmd *left, struct cmd *right) {
    struct pipe_cmd *pipe;

    pipe = malloc(sizeof(*pipe));
    memset(pipe, 0, sizeof(*pipe));
    pipe->left = left;
    pipe->right = right;
    pipe->type = PIPE;

    return pipe;
}

struct back_cmd *back_cmd(void) {
    struct back_cmd *back;

    back = malloc(sizeof(*back));
    back->how_many = 0;
    back->type = BACK;

    return back;
}