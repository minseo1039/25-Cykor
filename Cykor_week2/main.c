#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <pwd.h>
#include "gen_func.h"
#include "color.h"

void main_window();
void print_path(Node *head);
int run_pipeline(char *cmd);
void parse_and_execute(char *command);

void main_window() {
    printf("===============================================\n");
    printf("2025.05.07.\n\nWelcome to min's shell!\n\n");
    printf("Please enter the command you want to execute.\n");
    printf("If you want to exit, please enter 'exit'.\n");
    printf("===============================================\n");
}

int run_pipeline(char *cmd) { // 파이프라인 실행 함수
    char *args[20];
    int cnt = 0;
    args[cnt] = strtok(cmd, "|"); // 명령어를 파이프로 분리
    while (args[cnt] != NULL) {
        ltrim(args[cnt]);
        cnt++;
        args[cnt] = strtok(NULL, "|"); // 다음 명령어로 이동
    }

    int fd[2], in_fd = STDIN_FILENO; // 초기 입력 파일 디스크립터
    pid_t pid;
    int status;

    for (int i = 0; i < cnt; i++) {
        pipe(fd); // 파이프 생성
        pid = fork();
        if (pid == 0) {
            dup2(in_fd, STDIN_FILENO);
            if (i != cnt - 1) // 마지막 명령어가 아닐 경우
                dup2(fd[1], STDOUT_FILENO);
            close(fd[0]); // 부모 프로세스 읽기 끝 닫음
            close(fd[1]); // 자식 프로세스 쓰기 끝 닫음

            char *argv[20];
            int j = 0;
            argv[j] = strtok(args[i], " "); // 명령어와 인자를 분리
            while (argv[j] != NULL) {
                argv[++j] = strtok(NULL, " ");
            }
            execvp(argv[0], argv); // 명령어 실행
            perror("execvp failed");
            exit(1);
        }
        else {
            waitpid(pid, &status, 0); // 자식 프로세스 대기
            close(fd[1]); // 부모 프로세스 쓰기 끝 닫음
            in_fd = fd[0]; // 다음 명령어의 입력으로 사용하기 위해 읽기 끝 저장
        }
    }
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1; // 자식 프로세스의 종료 상태 반환
}

void parse_and_execute(char *command) {
    char *token = strtok(command, ";");
    while (token != NULL) {
        // && 처리
        char *and_pos = strstr(token, "&&"); // "&&" 연산자 찾기
        if (and_pos) {
            *and_pos = '\0';
            char *next = and_pos + 2; // 다음 명령어 위치
            ltrim(token);
            ltrim(next);
            int status = run_pipeline(token);
            if (status == 0)
                run_pipeline(next); // 성공 시 다음 명령어 실행
            token = strtok(NULL, ";");
            continue; // 다음 명령어로 이동
        }

        // || 처리
        char *or_pos = strstr(token, "||");
        if (or_pos) {
            *or_pos = '\0';
            char *next = or_pos + 2;
            ltrim(token);
            ltrim(next);
            int status = run_pipeline(token);
            if (status != 0)
                run_pipeline(next); // 실패 시 다음 명령어 실행
            token = strtok(NULL, ";");
            continue;
        }

        // 백그라운드 처리
        int background = 0;
        char *bg_pos = strstr(token, "&");
        if (bg_pos) {
            *bg_pos = '\0'; // "&" 제거
            background = 1; // 백그라운드 실행 플래그 설정
        }
        ltrim(token);

        pid_t pid = fork();
        if (pid == 0) {
            run_pipeline(token); // 자식 프로세스에서 명령어 실행
            exit(0); // 자식 프로세스 종료
        }
        else {
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
    char hostname[256];
    struct passwd *pw;
    Node *path_list = NULL;
    pw = getpwuid(uid);
    gethostname(hostname, sizeof(hostname));
    path_list = load_current_path();
    main_window();

    while (1) {
        printf("%s%s", YELLOW, "min's Shell:"); // 쉘 이름
        printf("%s%s@%s%s:", GREEN, pw->pw_name, hostname, BLUE); // 사용자 이름과 호스트 이름
        print_path(path_list); // 현재 경로 출력
        printf("%s ", RESET);
        printf("$ ");
        if (fgets(command, sizeof(command), stdin) == NULL) { // EOF 처리
            break;
        }
        command[strcspn(command, "\n")] = 0; // 개행 문자 제거

        // 내장 명령어 처리
        if (strncmp(command, "cd ", 3) == 0) { // cd 명령어 처리
            char *path = command + 3; // "cd " 이후의 경로
            ltrim(path); // 공백 제거
            if (chdir(path) != 0) { // 경로 변경 실패
                perror("cd failed");
            }
            else {
                free_path_list(path_list); // 이전 경로 리스트 해제
                path_list = load_current_path(); // 새로운 경로 리스트 로드
            }
            continue;
        }
        if (strcmp(command, "pwd") == 0) { // 현재 경로 출력
            print_path(path_list);
            printf("\n");
            continue;
        }
        if (strcmp(command, "exit") == 0) { // 쉘 종료
            printf("Exit the shell.\n");
            break; // 종료
        }

        // 외부 명령어 및 연산자 처리
        parse_and_execute(command);
    }

    free_path_list(path_list); // 경로 리스트 해제
    return 0;
}
