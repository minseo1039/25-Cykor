#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <pwd.h>

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
int run_pipeline(char *cmd);

void main_window() {
    printf("===============================================\n");
    printf("2025.05.07.\n\nWelcome to min's shell!\n\n");
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

int run_pipeline(char *cmd) {
    char *args[20];
    int cnt = 0;
    args[cnt] = strtok(cmd, "|");
    while (args[cnt] != NULL) {
        ltrim(args[cnt]);
        cnt++;
        args[cnt] = strtok(NULL, "|");
    }

    int fd[2], in_fd = STDIN_FILENO;
    pid_t pid;
    int status;

    for (int i = 0; i < cnt; i++) {
        pipe(fd);
        pid = fork();
        if (pid == 0) {
            dup2(in_fd, STDIN_FILENO);
            if (i != cnt - 1)
                dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);

            char *argv[20];
            int j = 0;
            argv[j] = strtok(args[i], " ");
            while (argv[j] != NULL) {
                argv[++j] = strtok(NULL, " ");
            }
            execvp(argv[0], argv);
            perror("execvp failed");
            exit(1);
        } else {
            waitpid(pid, &status, 0);
            close(fd[1]);
            in_fd = fd[0];
        }
    }
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

void parse_and_execute(char *command) {
    char *token = strtok(command, ";");
    while (token != NULL) {
        // && 처리
        char *and_pos = strstr(token, "&&");
        if (and_pos) {
            *and_pos = '\0';
            char *next = and_pos + 2;
            ltrim(token);
            ltrim(next);
            int status = run_pipeline(token);
            if (status == 0) run_pipeline(next);
            token = strtok(NULL, ";");
            continue;
        }

        // || 처리
        char *or_pos = strstr(token, "||");
        if (or_pos) {
            *or_pos = '\0';
            char *next = or_pos + 2;
            ltrim(token);
            ltrim(next);
            int status = run_pipeline(token);
            if (status != 0) run_pipeline(next);
            token = strtok(NULL, ";");
            continue;
        }

        // 백그라운드 처리
        int background = 0;
        char *bg_pos = strstr(token, "&");
        if (bg_pos) {
            *bg_pos = '\0';
            background = 1;
        }
        ltrim(token);

        pid_t pid = fork();
        if (pid == 0) {
            run_pipeline(token);
            exit(0);
        } else {
            if (!background) {
                waitpid(pid, NULL, 0);
            }
        }

        token = strtok(NULL, ";");
    }
}

int main() {
    uid_t uid = getuid();
    char command[256];
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

        // 내장 명령어 처리
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

        // 외부 명령어 및 연산자 처리
        parse_and_execute(command);
    }

    free_path_list(path_list);
    return 0;
}
