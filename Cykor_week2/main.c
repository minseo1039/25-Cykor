#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/wait.h>

typedef struct node {
    char folder[256];
    struct node *next;
} Node;

struct passwd *pw;
char hostname[256];
Node *path_list = NULL;

void main_window();
Node *create_node(char *folder);
void free_path_list(Node *head);
Node *load_current_path();
void print_path(Node *head);
void ltrim(char *str);
void pipe_command(char *command);
void run_command(char *args[]);

void main_window() {
    printf("===============================================\n");
    printf("2025.05.07.\n");
    printf("\nWelcome to min's shell!\n\n");
    printf("Please enter the command you want to execute.\n");
    printf("If you want to exit, please enter 'exit'.\n");
    printf("===============================================\n");
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

void ltrim(char *str) {
    char *p = str;
    while (*p && isspace((unsigned char)*p)) p++;
    memmove(str, p, strlen(p) + 1);
}

void pipe_command(char *command) {
    char *args[20];
    int cnt = 0;
    args[0] = strtok(command, "|");
    while (args[cnt] != NULL) {
        ltrim(args[cnt]);
        args[++cnt] = strtok(NULL, "|");
    }
    run_command(args);
}

void run_command(char *args[]) {
    int i = 0;
    if (args[i + 1] == NULL) {
        // 마지막 명령어도 fork → execvp → waitpid 처리
        pid_t pid = fork();
        if (pid == 0) {
            char *cmd_args[20];
            int j = 0;
            cmd_args[j] = strtok(args[i], " ");
            while (cmd_args[j] != NULL) {
                cmd_args[++j] = strtok(NULL, " ");
            }
            execvp(cmd_args[0], cmd_args);
            perror("execvp failed");
            exit(1);
        } else {
            waitpid(pid, NULL, 0);
        }
    } else {
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe failed");
            exit(1);
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);

            char *cmd_args[20];
            int j = 0;
            cmd_args[j] = strtok(args[i], " ");
            while (cmd_args[j] != NULL) {
                cmd_args[++j] = strtok(NULL, " ");
            }
            execvp(cmd_args[0], cmd_args);
            perror("execvp failed");
            exit(1);
        } else {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            run_command(&args[i + 1]);
            waitpid(pid, NULL, 0);  // 부모가 자식 종료 기다림
        }
    }
}

int main(void) {
    uid_t uid = getuid();
    char command[256];
    int stdin_backup = dup(STDIN_FILENO);
    pw = getpwuid(uid);
    gethostname(hostname, sizeof(hostname));
    path_list = load_current_path();
    main_window();

    while (1) {
        printf("%s@%s:", pw->pw_name, hostname);
        print_path(path_list);
        printf("$ ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }
        command[strcspn(command, "\n")] = 0;

        // ----- 내장 명령어 처리 -----
        if (strncmp(command, "cd ", 3) == 0) {
            char *path = command + 3;
            ltrim(path);
            if (chdir(path) != 0) {
                perror("cd failed");
            } else {
                free_path_list(path_list);
                path_list = load_current_path();
            }
            continue;
        }
        if (strcmp(command, "pwd") == 0) {
            print_path(path_list);
            printf("\n");
            continue;
        }
        if (strcmp(command, "exit") == 0) {
            printf("Exit the shell.\n");
            break;
        }

        // ----- 외부 명령어 처리 -----
        pipe_command(command);
        dup2(stdin_backup, STDIN_FILENO);
    }

    free_path_list(path_list);
    return 0;
}
