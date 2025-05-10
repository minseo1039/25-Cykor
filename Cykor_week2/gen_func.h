#ifndef GEN_FUNC_H
#define GEN_FUNC_H

typedef struct node {
    char folder[256];
    struct node *next;
} Node;

void ltrim(char *str);
Node *create_node(char *folder);
void free_path_list(Node *head);
Node *load_current_path();
void print_path(Node *head);

#endif
