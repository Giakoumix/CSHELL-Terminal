
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aliases.h"

#include "ADTMap.h"
#include "ADTVector.h"

static Map alias;

int compare_string(Pointer a, Pointer b) {
    char *str1 = a; char *str2 = b;
    return strcmp(str1, str2);
}

void init_alias(void) {
    alias = map_create(compare_string, free, free);
    map_set_hash_function(alias, hash_string);
}

int check_alias(char *line) {
    char *temp = malloc(strlen(line)+1);
    strcpy(temp, line);
    char *token = strtok(temp, " ");
    // printf("Checking for alias\n");
    if (!strcmp(token, "createalias")) {
        create_alias(line);
        free(temp);
        return 1;
    }
    else if (!strcmp(token, "destroyalias")) {
        destroy_alias(line);
        free(temp);
        return 1;
    }
    else {
        free(temp);
        return 0;
    }
}

void create_alias(char *line) {
    char *temp = malloc(strlen(line)+1);
    strcpy(temp, line);
    char *token = strtok(temp, " ");
    token = strtok(NULL, " ");

    if (token == NULL) {
        free(temp);
        fprintf(stderr, "Not enough arguments to createalias");
        return;
    }

    char *al = malloc(strlen(token)+1);
    strcpy(al, token);

    token = strtok(NULL, " ");
    if (token == NULL) {
        free(temp);
        free(al);
        fprintf(stderr, "Not enough arguments to createalias");
        return;
    }
    char *str = malloc(strlen(token)+1);
    strcpy(str, token);
    // printf("Just before inserting into the map\n");
    map_insert(alias, al, str);
    free(temp);
}

char *replace_alias(char *line) {
    char *temp = malloc(strlen(line)+1);
    strcpy(temp, line);
    char *token = strtok(temp, " ");

    Vector vec = vector_create(0, free);
    while (token != NULL) {
        char *str = malloc(strlen(token)+1);
        strcpy(str, token);
        vector_insert_last(vec, str);
        token = strtok(NULL, " ");
    }

    int length = 0;
    char *found;

    for (int i = 0 ; i < vector_size(vec) ; i++) {
        token = vector_get_at(vec, i);
        if ((found = map_find(alias, token)) != NULL) {
            char *str = malloc(strlen(found)+1);
            strcpy(str, found);
            vector_set_at(vec, i, str);
        }
        token = vector_get_at(vec, i);
        length += strlen(token);
    }

    length += vector_size(vec);
    char *new_str = malloc(length);
    *new_str = '\0';

    for (int i = 0 ; i < vector_size(vec) ; i++) {
        token = vector_get_at(vec, i);
        strcat(new_str, token);
        strcat(new_str, " ");
    }

    free(line); 
    return new_str;
}

void destroy_alias(char *line) {
    char *temp = malloc(strlen(line)+1);
    strcpy(temp, line);
    char *token = strtok(temp, " ");

    token = strtok(NULL, " ");
    if (token == NULL) {
        fprintf(stderr, "Not enough arguments to destroyalias\n");
        free(temp);
        return;
    }

    if (strtok(NULL, " ") != NULL) {
        fprintf(stderr, "Too many arguments to destroyalias");
        free(temp);
        return;
    }

    map_remove(alias, token);
}

void destruct_alias() {
    map_destroy(alias);
}