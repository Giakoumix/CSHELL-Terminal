
#include "shell.h"

int execute(struct cmd *cmd) {
    
    int p[2], fd_in, fd_out, pid1, pid2;
    struct exec_cmd *exec;
    struct redir_cmd *redir;
    struct pipe_cmd *pipe_c;
    struct back_cmd *back;

    if (cmd == NULL) {
        perror("Error in execution");
        return ERROR;
    }

    switch (cmd->type)
    {
    case EXEC:
        exec = (struct exec_cmd *) cmd;

        execvp(exec->argv[0], exec->argv);
        perror("Exec");
        break;
    case REDIR:
        redir = (struct redir_cmd *) cmd;

        if (redir->file_in != NULL) {
            close(READ);
            fd_in = open(redir->file_in, O_RDONLY, 0666);
            if (fd_in == -1) {
                perror("This file doesnt exist");
                exit(1);
            }
        }
        if (redir->file_out != NULL) {
            close(WRITE);
            fd_out = open(redir->file_out, O_CREAT | O_WRONLY | O_TRUNC, 0666);
            if (fd_out == -1) {
                perror("Cant open the file");
                exit(1);
            }
        }
        execute((struct cmd *) redir->cmd);
        break;
    case PIPE: 
        pipe_c = (struct pipe_cmd *) cmd;

        if (pipe(p)==-1) {
            perror("Pipe call");
            exit(1);
        }

        if ((pid1=fork())==0) {
            close(p[READ]);
            dup2(p[WRITE], WRITE);
            close(p[WRITE]);
            execute(pipe_c->left);
        }
        if ((pid2=fork())==0) {
            close(p[WRITE]);
            dup2(p[READ], READ);
            close(p[READ]);
            execute(pipe_c->right);
        }

        close(p[READ]);
        close(p[WRITE]);

        wait(0);
        wait(0);

        break;
    case BACK:
        back = (struct back_cmd *) cmd;
        int *pid = malloc(back->how_many*sizeof(*pid));
        if (fork() != 0) {
            static struct sigaction act;
            sigfillset(&(act.sa_mask));

            for (int i = 0 ; i < back->how_many ; i++) {
                if ((pid[i]=fork()) != 0) {
                    execute(back->list[i]);
                }
            }

            while(wait(NULL) > 0);
            free(pid);
            exit(0);
        }

        break;
    default:
        break;
    }

    exit(0);
}