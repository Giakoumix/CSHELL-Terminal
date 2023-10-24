
#include "prompt.h"

void print_prompt(void) {
    char *prompt_str = calloc(PROMPT_SIZE+1, 1);
    getcwd(prompt_str, PROMPT_SIZE);
    prompt_str[strlen(prompt_str)] = '>';
    printf("%s", prompt_str);
    free(prompt_str);
}

char *get_prompt_line(void) {
    char *command_str = calloc(CMD_SIZE, 1); 
    fgets(command_str, CMD_SIZE, stdin);
    command_str[strlen(command_str)-1] = '\0';
    return command_str;
}