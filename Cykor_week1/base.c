
/*  call_stack
    
    실제 시스템에서는 스택이 메모리에 저장되지만, 본 과제에서는 `int` 배열을 이용하여 메모리를 구현합니다.
    원래는 SFP와 Return Address에 실제 가상 메모리 주소가 들어가겠지만, 이번 과제에서는 -1로 대체합니다.
    
    int call_stack[]      : 실제 데이터(`int 값`) 또는 `-1` (메타데이터 구분용)을 저장하는 int 배열 
    char stack_info[][]   : call_stack[]과 같은 위치(index)에 대한 설명을 저장하는 문자열 배열

    ==========================call_stack 저장 규칙==========================
    매개 변수 / 지역 변수를 push할 경우   : int 값 그대로
    Saved Frame Pointer 를 push할 경우  : call_stack에서의 index
    반환 주소값을 push할 경우       : -1
    =======================================================================
    

    ==========================stack_info 저장 규칙==========================
    매개 변수 / 지역 변수를 push할 경우        : 변수에 대한 설명
    Saved Frame Pointer 를 push할 경우  : 어떤 함수의 SFP인지
    반환 주소값을 push할 경우                 : "Return Address"
    ========================================================================
*/
#include <stdio.h>
#include <string.h>
#define STACK_SIZE 50 // 최대 스택 크기

int     call_stack[STACK_SIZE];         // Call Stack을 저장하는 배열
char    stack_info[STACK_SIZE][20];     // Call Stack 요소에 대한 설명을 저장하는 배열

/*  SP (Stack Pointer), FP (Frame Pointer)

    SP는 현재 스택의 최상단 인덱스를 가리킵니다.
    스택이 비어있을 때 SP = -1, 하나가 쌓이면 `call_stack[0]` -> SP = 0, `call_stack[1]` -> SP = 1, ...

    FP는 현재 함수의 스택 프레임 포인터입니다.
    실행 중인 함수 스택 프레임의 sfp를 가리킵니다.
*/
int SP = -1;
int FP = -1;
int local_index = 1; // 로컬 변수의 인덱스 순서서

void init(); // 스택 초기화 함수
void func1(int arg1, int arg2, int arg3);
void func2(int arg1, int arg2);
void func3(int arg1);
void prolog(int current_FP, char *info, int local); // 프롤로그
void epillogeu(); // 에필로그
void push(int value, char *info); // 스택에 푸쉬
void pop(); // 스택에서 팝

/*  
    현재 call_stack 전체를 출력합니다.
    해당 함수의 출력 결과들을 바탕으로 구현 완성도를 평가할 예정입니다.
*/

void init()
{
    int i;
    for(i=0; i<STACK_SIZE; i++) // call_stack의 모든 값을 -1로 초기화
    {
        call_stack[i] = -1;
    }
}

void print_stack()
{
    if (SP == -1)
    {
        printf("Stack is empty.\n");
        return;
    }

    printf("====== Current Call Stack ======\n");
    
    for (int i = SP; i >= 0; i--)
    {
        if (call_stack[i] != -1)
            printf("%d : %s = %d", i ,stack_info[i], call_stack[i]);
        else
            printf("%d : %s", i, stack_info[i]);

        if (i == SP)
            printf("    <=== [esp]\n");
        else if (i == FP)
            printf("    <=== [ebp]\n");
        else
            printf("\n");
    }
    printf("================================\n\n");
}


//func 내부는 자유롭게 추가해도 괜찮으나, 아래의 구조를 바꾸지는 마세요
void func1(int arg1, int arg2, int arg3)
{
    int var_1 = 100;
    int i;
    int *p = &var_1; // 로컬 변수의 첫 번째 주소를 저장
    
    // func1의 스택 프레임 형성 (함수 프롤로그 + push)
    push(arg3, "arg3"); // arg3, arg2, arg1 순서로 push
    push(arg2, "arg2");
    push(arg1, "arg1");
    prolog(FP, "func1 SFP", 1); // SFP, Return Address 순서로 push 그리고 로컬 변수 1개
    for(i=0; i<1; i++) // 로컬 변수 1개 stack에 추가
    {
        call_stack[SP-i] = *p;
        sprintf(stack_info[SP-i], "var_%d", local_index++);
        p += 1; // 다음 로컬 변수의 주소로 이동
    }
    print_stack();
    func2(11, 13);
    // func2의 스택 프레임 제거 (함수 에필로그 + pop)
    epillogeu();
    pop(); // callee의 arg1, arg2 제거
    pop();
    print_stack();
}


void func2(int arg1, int arg2)
{
    int var_2 = 200;
    int i;
    int *p = &var_2; // 로컬 변수의 첫 번째 주소를 저장

    // func2의 스택 프레임 형성 (함수 프롤로그 + push)
    push(arg2, "arg2"); // arg2, arg1 순서로 push
    push(arg1, "arg1");
    prolog(FP, "func2 SFP", 1); // SFP, Return Address 순서로 push 그리고 로컬 변수 1개
    for(i=0; i<1; i++)
    {
        call_stack[SP-i] = *p;
        sprintf(stack_info[SP-i], "var_%d", local_index++);
        p += 1; // 다음 로컬 변수의 주소로 이동
    }
    print_stack();
    func3(77);
    // func3의 스택 프레임 제거 (함수 에필로그 + pop)
    epillogeu();
    pop(); // callee의 arg1 제거
    print_stack();
}


void func3(int arg1)
{
    int var_3 = 300;
    int var_4 = 400;
    int i;
    int *p = &var_3; // 로컬 변수의 첫 번째 주소를 저장

    // func3의 스택 프레임 형성 (함수 프롤로그 + push)
    push(arg1, "arg1"); // arg1 순서로 push
    prolog(FP, "func3 SFP", 2); // SFP, Return Address 순서로 push 그리고 로컬 변수 2개
    for(i=0; i<2; i++) // 로컬 변수 2개 stack에 추가
    {
        call_stack[SP-i] = *p;
        sprintf(stack_info[SP-i], "var_%d", local_index++);
        p += 1; // 다음 로컬 변수의 주소로 이동
    }
    print_stack();
}


void prolog(int current_FP, char *info, int local)
{
    push(-1, "Return Address"); // Return Address push
    push(current_FP, info); // SFP push
    FP = SP;
    if(SP + local > STACK_SIZE) // 로컬 변수의 개수가 STACK_SIZE를 넘어가는 경우
        perror("I cann't push the local values\n");
    SP += local;
}


void epillogeu()
{
    int temp = SP, i;
    for(i=FP; i<=temp+1; i++) // Return Address까지 pop
    {
        if(strstr(stack_info[SP], "SFP")) // SFP를 찾으면
            FP = call_stack[SP]; // FP를 SFP로 변경
        pop();
    }
}


void push(int value, char *info)
{
    if(SP >= STACK_SIZE-1) // 스택이 가득 찬 경우
        perror("Stack is Full!\n");
    call_stack[++SP] = value;
    strcpy(stack_info[SP], info);
}


void pop()
{
    call_stack[SP] = -1; // pop된 call_stack을 초기화
    stack_info[SP][0] = 0; // pop된 stack_info를 초기화
    SP--;
}


//main 함수에 관련된 stack frame은 구현하지 않아도 됩니다.
int main()
{
    init(); // call_stack 초기화
    func1(1, 2, 3);
    // func1의 스택 프레임 제거 (함수 에필로그 + pop)
    epillogeu();
    pop(); // callee의 arg1, arg2, arg3 제거
    pop();
    pop();
    print_stack();
    return 0;
}