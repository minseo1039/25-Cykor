#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pwd.h>
#include "gen_func.h"

void ltrim(char *str) {
    char *p = str;
    while (*p && isspace((unsigned char)*p))
        p++; // 공백 문자 건너뛰기
    memmove(str, p, strlen(p) + 1); // 나머지 문자열을 앞으로 이동
}

Node *create_node(char *folder) {
    Node *new_node = (Node *)malloc(sizeof(Node)); // 메모리 할당
    strcpy(new_node->folder, folder); // 폴더 이름 복사
    new_node->next = NULL; // 다음 노드 초기화
    return new_node; // 새 노드 반환
}

void free_path_list(Node *head) {
    Node *current = head;
    Node *next;
    while (current != NULL) {
        next = current->next;
        free(current); // 현재 노드 메모리 해제
        current = next;
    }
}

Node *load_current_path() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    Node *head = NULL; // 리스트의 헤드 포인터
    Node *tail = NULL; // 리스트의 꼬리 포인터
    char *folder = strtok(cwd, "/");
    while (folder != NULL) {
        Node *new_node = create_node(folder);
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        }
        else { // 리스트가 비어있지 않은 경우
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
