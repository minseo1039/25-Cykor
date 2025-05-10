#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pwd.h>
#include "gen_func.h"

void ltrim(char *str) {
    char *p = str;
    while (*p && isspace((unsigned char)*p)) p++;
    memmove(str, p, strlen(p) + 1);
}

Node *create_node(char *folder) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    strcpy(new_node->folder, folder);
    new_node->next = NULL;
    return new_node;
}

void free_path_list(Node *head) {
    Node *current = head;
    Node *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

Node *load_current_path() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    Node *head = NULL;
    Node *tail = NULL;
    char *folder = strtok(cwd, "/");
    while (folder != NULL) {
        Node *new_node = create_node(folder);
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        folder = strtok(NULL, "/");
    }
    return head;
}

void print_path(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("/%s", current->folder);
        current = current->next;
    }
}
