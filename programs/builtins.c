
#include "builtins.h"
#include "history.h"

int check_builtin(char *line) {
    int length = strlen(line);
    char *temp = malloc(length+1);

    strncpy(temp, line, length);
    temp[length-1] = '\0';
    line[length-1] = '\0';
    
    // printf("After copied temp is %s\n", temp);
    char *token = strtok(temp, " ");
    
    if (token == NULL) {
        fprintf(stderr, "Not enough arguments\n");
        free(temp);
        return 0;
    }

    add_history(line);

    if (!strcmp(token, "cd")) {
        change_dir(line);
        return 1;
    }
    if (!strcmp(token, "pwd")) {
        print_working_dir(line);
        return 1;
    }
    if (!strcmp(token, "myhistory")) {
        show_history();
        return 1;
    }
    if (!strcmp(token, "quit")) {
        return 2;
    }

    free(temp);

    return 0;
}

void change_dir(char *line) {
    char *temp = malloc(strlen(line)+1);
    strcpy(temp, line);
    char *token = strtok(temp, " ");
    token = strtok(NULL, " ");
    
    if (strtok(NULL, " ") != NULL) {
        perror("Too many arguments");
        return;
    }

    if (chdir(token) == -1) {
        perror("Not a directory");
    }

    free(temp);
}

void print_working_dir(char *line) {
    char *temp = malloc(strlen(line)+1);
    strcpy(temp, line);
    char *token = strtok(temp, " ");
    token = strtok(NULL, " ");

    if (strtok(NULL, " ") != NULL) {
        perror("Too many arguments");
        return;
    }

    char *wd = malloc(100);
    getcwd(wd, 100);

    printf("%s\n", wd);

    free(temp);
    free(wd);
}