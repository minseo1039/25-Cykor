#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pwd.h>

typedef struct node{
    char folder[256]; // 폴더 이름
    struct node *next;
}Node;

struct passwd *pw;
char hostname[256];

void main_window() // 초기 화면을 출력하는 함수
{
    printf("===============================================\n");
    printf("2025.05.07.\n");
    printf("\n");
    printf("Welcome to min's shell!\n");
    printf("\n");
    printf("Please enter the command you want to execute.\n");
    printf("If you want to exit, please enter 'exit'.\n");
    printf("===============================================\n");
    printf("%s@%s:", pw->pw_name, hostname);
}

Node *create_node(char *folder) // 노드를 생성하는 함수
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    strcpy(new_node->folder, folder);
    new_node->next = NULL;
    return new_node;    
}

Node *load_current_path()
{
    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd 실패");
        exit(1);
    }

    Node *head = NULL;
    Node *tail = NULL;

    // strtok로 '/' 기준 분리
    char *folder = strtok(cwd, "/");
    while(folder != NULL)
    {
        Node *new_node = create_node(folder);
        if(head == NULL)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
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

int main(void)
{
    uid_t uid = getuid();
    int i;
    Node *path_list = load_current_path();

    pw = getpwuid(uid);
    gethostname(hostname, sizeof(hostname));

    main_window();

    print_path(path_list);
    printf("$");
    printf("\n");
    return 0;
}