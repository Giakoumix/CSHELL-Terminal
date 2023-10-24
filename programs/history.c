
#include "history.h"

static List history;

void init_history(void) {
    history = list_create(free);
}

void add_history(char *line) {
    char *temp = malloc(strlen(line)+1);
    strcpy(temp, line);

    list_insert_next(history, LIST_EOF, temp);

    if (list_size(history) > 20)
        list_remove_next(history, LIST_BOF);
}

void show_history(void) {
    for (ListNode node = list_first(history) ;
         node != LIST_EOF ;
         node = list_next(history, node)) {
            printf("%s\n", (char *) list_node_value(history, node));
    }
}

char *history_n(char *line) {
    char *new_line;
    char *temp = malloc(strlen(line)+1);
    strcpy(temp, line);
    char *tok = strtok(temp, " ");

    if (strcmp(tok, "history")) {
        free(temp);
        return line;
    }

    tok = strtok(NULL, " ");
    if (!atoi(tok)) {
        free(temp);
        return NULL;
    }

    int n = 0;
    int num = atoi(tok);

    if (num < 0) {
        free(temp);
        return NULL;
    }

    for (ListNode node = list_first(history) ;
         node != LIST_EOF ;
         node = list_next(history, node)) {
            n++;
            if (num != n)
                continue;
            char *token = list_node_value(history, node);
            new_line = malloc(strlen(token));
            strcpy(new_line, token);
    }

    free(line);
    return new_line;
}

void destruct_history() {
    list_destroy(history);
}