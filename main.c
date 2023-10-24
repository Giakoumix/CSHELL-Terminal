
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "commands.h"
#include "parser.h"
#include "shell.h"
#include "builtins.h"
#include "prompt.h"
#include "aliases.h"
#include "history.h"

void catchinterrupt(int signo) {
    printf("\n");
}

int main(void) {

    pid_t pid;
    char *command_str;
    static struct sigaction act;

    init_alias(); 
    init_history();

    act.sa_handler = &catchinterrupt;
    sigfillset(&(act.sa_mask));

    sigaction(SIGINT, &act, NULL);
    sigaction(SIGTSTP, &act, NULL);

    while(1) {
        print_prompt();
        command_str = get_prompt_line();
        
        if (!strcmp(command_str, ""))
            continue;

        if ((command_str = history_n(command_str)) == NULL)
            continue;

        if (check_alias(command_str))
            continue;

        command_str = replace_alias(command_str);

        int built_in = check_builtin(command_str);

        if (built_in == 1)
            continue;
        else if (built_in == 2)
            break;

        struct cmd *command = parse_back(command_str);

        if ((pid=fork()) == 0) {
            execute(command);
        }
        wait(0);
        free(command_str);
        destruct_command(command);
    }   

    destruct_alias();
    destruct_history();
    while(wait(NULL) > 0);

    return 0;
}