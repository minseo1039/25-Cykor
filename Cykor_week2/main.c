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
}

Node *create_node(char *folder) // 노드를 생성하는 함수
{
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

Node *load_current_path()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

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
    int i, cnt=0;
    char command[256];
    Node *path_list = load_current_path();

    pw = getpwuid(uid);
    gethostname(hostname, sizeof(hostname));

    main_window();

    while(1)
    {
        char *args[20];
        char *ptr;
        cnt = 0;
        printf("%s@%s:", pw->pw_name, hostname);
        print_path(path_list);
        printf("$");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // 개행 문자 제거
        args[0] = strtok(command, " ");
        while(args[cnt] != NULL)
        {
            args[++cnt] = strtok(NULL, " ");
        }
        if(strcmp(args[0], "exit") == 0) // exit 입력 시 종료
        {
            printf("Exit the shell.\n");
            break;
        }
        else if(strcmp(args[0], "pwd") == 0) // pwd 입력 시 현재 경로 출력
        {
            print_path(path_list);
            printf("\n");
        }
        else if(strcmp(args[0], "cd") == 0) // cd 입력 시 경로 변경
        {
            if(chdir(args[1]) == -1) // 경로 변경 실패 시 오류 메시지 출력
            {
                perror("cd failed");
            }
            else // 경로 변경 성공 시 경로 리스트 업데이트
            {
                free_path_list(path_list); // 기존 경로 리스트 메모리 해제
                path_list = load_current_path(); // 새로운 경로 리스트 로드
            }
        }
        else // 잘못된 명령어 입력 시 오류 메시지 출력
        {
            printf("Invalid command. Please try again.\n");
        }
    }
    return 0;
}