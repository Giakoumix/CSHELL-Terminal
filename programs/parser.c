
#include "parser.h"
#include "commands.h"

struct cmd *parse_back(char *line) {
    if (strstr(line, "&;") == NULL)
        return parse_pipe(line);

    int back_num = 0; 
    char *back_lines[BACKCMD];

    for (int i = 0 ; i < BACKCMD ; i++)
        back_lines[i] = malloc(MAX_PIPECMD);

    // Initialize every background line
    char *token = malloc(MAX_PIPECMD);
    token = strtok(line, "&;");
    while (token != NULL) {
        strcpy(back_lines[back_num], token);
        back_num++;
        token = strtok(NULL, "&;");
    }

    struct back_cmd *back = back_cmd();
    back->how_many = back_num;

    // Initialize the background list with the commands
    for (int i = 0 ; i < back_num ; i++) {
        back->list[i] = parse_pipe(back_lines[i]);
    }

    return (struct cmd *) back;
}

struct cmd *parse_pipe(char *line) {
    if (strchr(line, '|') == NULL) {
        return parse_singlecmd(line);
    }

    int pipe_num = 0;
    char *pipe_lines[MAX_PIPES];

    for (int i = 0 ; i < MAX_PIPES ; i++)
        pipe_lines[i] = malloc(MAX_SINGLCMD);
    
    // Initialize every pipe_line
    char *token = malloc(MAX_SINGLCMD);
    token = strtok(line, "|");
    while (token != NULL) {
        strcpy(pipe_lines[pipe_num], token);
        pipe_num++;
        token = strtok(NULL, "|");
    }

    struct pipe_cmd *pipe = pipe_cmd(NULL, NULL);
    struct pipe_cmd *temp = pipe;
    
    // Create the data structure for commands
    for (int i = 0 ; i < pipe_num ; i++) {
        if (!strcmp(pipe_lines[i], "")) {
            perror("Wrong Syntax");
            return NULL;
        }
        if (pipe_num-i == 1) {
            temp->right = parse_singlecmd(pipe_lines[i]);
            if (temp->right == NULL) {
                perror("Wrong Syntax");
                return NULL;
            }
        }
        else {
            temp->left = parse_singlecmd(pipe_lines[i]);
            if (pipe_num-i-1>1) {
                temp->right = (struct cmd *) pipe_cmd(NULL, NULL);
                temp = (struct pipe_cmd *) temp->right;
            }
        }
    }

    return (struct cmd *) pipe;
}

struct cmd *parse_singlecmd(char *line) {
    if (strchr(line, '>') == NULL && strchr(line, '<') == NULL)
        return (struct cmd *) parse_exec(line);
    
    int pos = 0;
    while(line[pos] != '>' && line[pos] != '<')
        pos++;

    char *first_half = malloc(pos);
    strncpy(first_half, line, pos);
    first_half[pos] ='\0';
    struct exec_cmd *exec = parse_exec(first_half);
    
    line = line + pos;

    char *token = strtok(line, " ");
    char *arr[4];
    for (int i = 0 ; i < 4 ; i++)
        arr[i] = malloc(MAX_ARGUMENT);
    int n = 0;

    while(token != NULL) {
        strcpy(arr[n], token);
        if (++n == 5) { // Remember to free pointers
            perror("Too many arguments");
            return NULL;
        }
        token = strtok(NULL, " ");
    }

    char *file_in = NULL;
    char *file_out = NULL;
    
    // In file
    if (!strcmp(arr[0], "<")) {
        if (arr[1] == NULL) {
            perror("Wrong Syntax 1");
            return NULL;
        }
        file_in = arr[1];
    }
    else {
        if (!strcmp(arr[1], "")) {
            perror("Wrong Syntax 2");
            return NULL;
        }
        file_out = arr[1];
    }
    if (arr[2] != NULL) {
        if (arr[3] == NULL) {
            perror("Wrong Syntax 3");
            return NULL;
        }
        if (!strcmp(arr[2], ">")) {
            if (arr[3] == NULL)
                file_out = NULL;
            else
                file_out = arr[3];
        }
        else if (!strcmp(arr[2], "<")) {
            if (arr[3] == NULL)
                file_in = NULL;
            else
                file_in = arr[3];
        }
    }

    if (exec == NULL) {
        perror("Wrong Syntax 5");
        return NULL;
    }
    
    return (struct cmd *) redir_cmd(exec, file_in, 25, file_out, 25);
}

struct exec_cmd *parse_exec(char *line) {
    struct exec_cmd *exec = exec_cmd();
    
    char *token = strtok(line, " ");
    int n = 0;

    while (token != NULL) {
        strcpy(exec->argv[n], token);
        n++;
        token = strtok(NULL, " ");
    }

    glob_t gstruct;
    Vector args = vector_create(0, NULL);
    bool match = false;

    for (int i = 0 ; i < n ; i++) {
        vector_insert_last(args, exec->argv[i]);
    }

    // Getting all matched files
    char *dir_file = malloc(170);
    for (int i = 0 ; i < n ; i++) {
        if (strchr(exec->argv[i], '*') || strchr(exec->argv[i], '?')) {
            
            getcwd(dir_file, 170);
            DIR *dir;
            struct dirent *dirent;
            struct stat st;

            if ((dir = opendir(dir_file)) != NULL) {
                
                while ((dirent=readdir(dir)) != NULL) {
                    int r = fnmatch(exec->argv[i], dirent->d_name, 0);
                    if (!r) {
                        match = true;
                        vector_insert_last(args, dirent->d_name);
                    }
                }
            } 
            
        }  
    }
    free(dir_file);

    int end = n; 
    char **temp_argv = exec->argv;

    // Appending all matched files to the argv array
    if (match) {
        int new_size = vector_size(args);
        exec->argv = malloc((new_size+1) * sizeof(*(exec->argv)));

        int k = 0;
        for (VectorNode node = vector_first(args);
            node != VECTOR_EOF;
            node = vector_next(args, node)) {
                char *str = vector_node_value(args, node);
                if (strchr(str, '*') == NULL && strchr(str, '?') == NULL) {
                    exec->argv[k] = malloc(strlen(str)+1);
                    strcpy(exec->argv[k], str);
                    k++;
                }
        }
        exec->size = k;
        end = k;
        n = k;
        
        for (int i = 0 ; i < MAXARGS ; i++) 
            free(temp_argv[i]);
        free(temp_argv);
    }

    for (int j = n ; j < end+1 ; j++)
        exec->argv[j] = NULL;

    return exec;
}

void destruct_command(struct cmd *command) {
    struct exec_cmd *exec;
    struct redir_cmd *redir;
    struct pipe_cmd *pipe_c;
    struct back_cmd *back;

    if (command==NULL)
        return;
    
    switch (command->type)
    {
    case EXEC:
        exec = (struct exec_cmd *) command;
        for (int i = 0 ; i < exec->size ; i++) {
            free(exec->argv[i]);
        }
        free(exec->argv);
        free(exec);
        break;
    case REDIR:
        redir = (struct redir_cmd *) command;
        destruct_command( (struct cmd *) redir->cmd);
        free(redir->file_in);
        free(redir->file_out);
        free(redir);
        break;
    case PIPE:
        pipe_c = (struct pipe_cmd *) command;

        destruct_command((struct cmd *) pipe_c->left);
        destruct_command((struct cmd *) pipe_c->right);

        free(pipe_c);
        break;
    case BACK:
        back = (struct back_cmd *) command;

        for (int i = 0 ; i < back->how_many ; i++)
            destruct_command((struct cmd *) back->list[i]);
        
        free(back->list);
        free(back);
    default:
        break;
    }

    return;
}