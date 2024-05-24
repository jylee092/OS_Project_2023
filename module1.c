#pragma warning(disable : 4996)

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_STACK_SIZE 10 //스택 최대
#define MAX_CHAR 5

typedef struct memory {
    char address[5];
    char value[5];
} MEMORY; //메모리 주소, 값 저장 구조체

typedef char Element;

Element stack[MAX_STACK_SIZE][MAX_CHAR];
int top;
int idx=0;

char DR[5] = "\0";
char code[10] = "\0";
MEMORY memory[10] = { 0 };

char R[5] = "\0";
char num[5] = "\0";
char bin[5] = "\0";
char b_4bit[5] = "0000";

void init_stack() {
    top = -1;
} //스택 초기화

int IsEmpty() {
    return top == -1;
}
int IsFull() {
    return top >= MAX_STACK_SIZE - 1;
}

void push(Element* e) {
    if (IsFull())
        printf("스택이 가득 찼습니다.");
    else
        strcpy(stack[++top], e);
}

Element* pop() {
    if (IsEmpty())
        printf("스택이 비었습니다.");
    else
        return stack[top--];
}

Element* peek() {
    if (IsEmpty())
        printf("스택 공백 에러");   // underflow : top 이 초기값 -1 일때 
    else
        return stack[top];
}

int find_add_idx(MEMORY* M, char* add) {
    for (int i = 0; i < 10; i++) {
        if (!strncmp(add, M[i].address, 4)) {
            return i;
        }
    }
}

void num_2_4bit(char* num, char* num_4bit) {
    if (strlen(num) == 3) {
        strncpy(num_4bit + 1, num, strlen(num));
    }
    else if (strlen(num) == 2) {
        strncpy(num_4bit + 2, num, strlen(num));
    }
    else if (strlen(num) == 1) {
        strncpy(num_4bit + 3, num, strlen(num));
    }
    else {
        strncpy(num_4bit, num, strlen(num));
    }
}

void Load_Num() {
    strncpy(DR, code + 5, 4);
}

void Load() {
    strncpy(R, code + 5, 4);
    int i = find_add_idx(memory, R);
    strncpy(DR, memory[i].value, 4);
}

void Store() {
    strncpy(R, code + 5, 4);
    int i = find_add_idx(memory, R);
    strncpy(memory[i].value, DR, 4);
}

void PushNum() {
    strncpy(num, code + 5, 4);
    push(num);
}

void Push() {
    push(DR);
}

int Pop() {
    strcpy(DR, pop(), 4);
}

void Add() {
    strcpy(R, pop(), 4);
    long add = strtol(R, NULL, 2);
    long DRadd = strtol(DR, NULL, 2);
    long Addvalue = add + DRadd;
    ltoa(Addvalue, bin, 2);
    num_2_4bit(bin, b_4bit);
    push(b_4bit);
}

void Sub() {
    strcpy(R, pop(), 4);
    long sub = strtol(R, NULL, 2);
    long DRSub = strtol(DR, NULL, 2);
    long Subvalue = sub - DRSub;
    ltoa(Subvalue, bin, 2);
    num_2_4bit(bin, b_4bit);
    push(b_4bit);
}

void Write_Address() {
    strncpy(memory[idx].address, code + 5, 4);
    strcpy(memory[idx].value, stack[top]);
    idx++;
}

void PrintAddress() {
    strncpy(R, code + 5, 4);
    int i = find_add_idx(memory, R);
    printf("result : %s\n", memory[i].value);
}

int main() {
    FILE* fp;//읽을 파일(프로그램 언어) 넣을 포인터 변수

    fp = fopen("data.bi", "r");//코드 파일(프로그램 언어) 열기
    if (fp == NULL) {
        printf("file을 읽어오는데 실패하였습니다.");
        return 0;
    }

    init_stack();

    while ((fgets(code, sizeof(code), fp)) != NULL) {

        if (code == NULL) break;

        if (!strncmp("0000", code, 4)) { // strncmp는 문자열의 원하는 부분까지 비교 -> 같으면 0을 반환
            Load_Num();
        }

        if (!strncmp("0001", code, 4)) {
            Load();
        }

        if (!strncmp("0010", code, 4)) {
            Store();
        }

        if (!strncmp("0011", code, 4)) {
            PushNum();
        }

        if (!strncmp("0100", code, 4)) {
            Push();
        }

        if (!strncmp("0101", code, 4)) {
            Pop();
        }

        if (!strncmp("0110", code, 4)) {
            Add();
        }

        if (!strncmp("0111", code, 4)) {
            Sub();
        }

        if (!strncmp("1000", code, 4)) {
            Write_Address();
        }

        if (!strncmp("1001", code, 4)) {
            PrintAddress();
        }
    }
    fclose(fp);
    return 0;
}

