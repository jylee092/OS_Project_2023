#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//문자열 마지막에 \0 잊지말기

typedef struct memory {
    char add_name[2];
    char addess[5];
} MEMORY; //메모리 구조체. 메모리 주소 이름, 주소(이름과 실제 주소값을 연결시키기 위해 [ex] "A = 0111")


void put_at_file(char* A, FILE* B) {
    fputs(A, B);
    fputs("\n", B);
} //파일에 바이너리 코드 문자열을 저장하는 함수

void find_adress_to_addName(char* A, MEMORY* B, char* C, int *find) {
    for (int i = 0; i < 10; i++) {
        if (!strncmp(A, B[i].add_name, 1)) {
            strcat(C, B[i].addess);
            *find = 1;
            break;
        }
    }
}
//"메모리 구조체 배열(B)에 저장된 메모리 이름(add_name)"과 "코드(프로그램 언어 1줄)로부터 추출한 메모리 이름(A)"을 비교
//메모리 구조체 배열에서 일치하는 주소 이름을 찾아 주소값(address)를 찾음. 바이너리 코드 문자열(C) 뒷부분에 추가

void load_add_or_num(int *find, char *add, char *load_add, char *load_num, FILE* A) {
    if (*find == 1) put_at_file(load_add, A); //LOAD [주소 이름] 문자열을 파일에 저장
    else if (*find == 0) {
        //printf("%s\n", add);
        long num = strtol(add, NULL, 10); //add(주소가 아닌 것으로 판명남 -> num)를 2진수로 변환
        char num_str[5];
        ltoa(num, num_str, 2); //2진수로 변환한 숫자를 num_str 문자열에 담음

        char num_4bit[5] = "0000"; //4비트로 변환한 num을 담을 문자열
        if (strlen(num_str) == 3) {
            strncpy(num_4bit + 1, num_str, 3);
        }
        else if (strlen(num_str) == 2) {
            strncpy(num_4bit + 2, num_str, 2);
        }
        else if (strlen(num_str) == 1) {
            strncpy(num_4bit + 3, num_str, 1);
        }
        else {
            strncpy(num_4bit, num_str, 4);
        } //4비트로 변환

        strcat(load_num, num_4bit); //LOAD NUM 바이너리 코드 문자열(load_num) 뒤에 num값 붙여주기
        put_at_file(load_num, A); //LOAD NUM 바이너리 코드 파일에 넣기
    }
    *find = 0;
} //LOAD [ ]와 LOAD NUM을 구분하기 위한 함수
// 만약 메모리 이름(add)을 찾았다면 LOAD [ ] 명령어 파일에 추가
// 만약 메모리 이름(add)을 찾지 못했다면 add를 long형 정수(2진수)로 변환(num값임), 2진수로 바귄 long형 변수를 문자열로 변환, LOAD NUM 명령어 만들어 파일에 추가

int main() {
    FILE* rfp;//읽을 파일(프로그램 언어) 넣을 포인터 변수
    FILE* wfp;//쓸 파일(바이너리 코드) 넣을 포인터 변수
    rfp = fopen("code.pl", "r");//코드 파일(프로그램 언어) 열기
    wfp = fopen("data.bi", "w");//쓸 파일(바이너리 코드) 열기


    char mcode[20];//명령어(프로그램 언어) 문자열을 담을 변수(한줄 전체)

    char add[5] = "\0";//명령어(프로그램 언어)에서 주소 추출한 것을 담음
    char num[5] = "\0";//명령어(프로그램 언어)에서 값 추출한 것을 담음

    MEMORY m_name_add[10] = {0};//메모리 구조체 배열(A, B, C... 등을 담을 메모리)
    int index = 0; //메모리 구조체 배열의 인덱스

    int *find = 0; //find_adress_to_addName 함수에서 메모리를 찾았을 경우(1), 찾지 못했을 경우(0)를 구분해주기 위한 변수


    if (rfp == NULL) {
        printf("파일 file.txt를 열 수 없습니다.\n");
        exit(0);
    }

    while (fgets(mcode, sizeof(mcode), rfp) != NULL)//파일 계속 읽어들이기. (mcode에 담은 명령어 문자열 한 줄이 NULL이 아니면)
    {
        if (!strncmp("WR", mcode, 2)) { //WRITE _, ____, ____ 의 앞 2글자 보고 구분

            strncpy(m_name_add[index].add_name, mcode + 6, 1);//구조체 배열[index]에 주소 이름(add_name) 추가 (WRITE "_", ____, ____)

            strncpy(num, mcode + 15, 4);//원래 코드로부터 값(num) 추출 (WRITE _, ____, "____")

            strncpy(add, mcode + 9, 4);//원래 코드로부터 주소(add) 추출(WRITE _, "____", ____)
            strncpy(m_name_add[index].addess, add, 4);//구조체 배열[index]에 추출한 주소 값(add = addess) 추가


            char push_n[10] = "0011 "; //PUSH NUM 바이너리 코드를 만들 문자열. 우선 PUSH를 가리키는 "0011 "(공백 포함)부터 채움.
            strcat(push_n, num); //문자열의 "0011 " 다음에 원래 코드로부터 추출한 값(num)을 넣어서 PUSH NUM 명령어를 완성시킴

            char write[10] = "1000 "; //WRITE Address 바이너리 코드를 만들 문자열. 우선 WRITE를 가리키는 "1000 "(공백 포함)부터 채움.
            strcat(write, add); //문자열의 "1000 " 다음에 원래 코드로부터 추출한 주소(add)를 넣어 WRITE Address 명령어를 완성시킴

            put_at_file(push_n, wfp);//PUSH NUM 문자열을 파일에 저장 (사용자 정의함수 put_at_file 사용 <- 맨 아래 있음)
            put_at_file(write, wfp);//WRITE Address 문자열을 파일에 저장
            put_at_file("0101", wfp);//POP 문자열을 파일에 저장

            index++;//구조체 배열 index 1 증가
        }//------------------------------------------- WRITE -------------------------------------------


        if (!strncmp("ST", mcode, 2)) { //STORE _ 의 앞 2글자 보고 구분
            char store_add[2] = "\0"; //원래 코드로부터 주소 이름 추출해서 넣을 문자열(store_add) 만들기(STORE "_")
            strncpy(store_add, mcode + 6, 1); //원래 코드로부터 주소 이름 추출, store_add에 넣기

            char store[10] = "0010 "; //STORE [주소이름] 바이너리 코드를 만들 문자열. 우선 STORE을 가리키는 "0010 "(공백 포함)부터 채움.

            find_adress_to_addName(store_add, m_name_add, store, &find); //메모리 구조체 배열에 저장된 메모리 이름(add_name)과 store_add를 비교, 일치하는 주소 이름을 찾아 주소값(address)를 찾음. 바이너리 코드 문자열(store = "0010 ") 뒷부분에 추가

            put_at_file(store, wfp); //STORE [주소이름] 문자열을 파일에 저장
        }//------------------------------------------- STORE -------------------------------------------


        if (!strncmp("PR", mcode, 2)) { //PRINT _ 의 앞 2글자 보고 구분
            char print_add[2] = "\0"; //원래 코드로부터 주소 이름 추출해서 넣을 문자열(print_add) 만들기(PRINT "_")
            strncpy(print_add, mcode + 6, 1); //원래 코드로부터 주소 이름 추출, print_add에 넣기

            char print[10] = "1001 "; //PRINT [주소이름] 바이너리 코드를 만들 문자열. 우선 PRINT을 가리키는 "1001 "(공백 포함)부터 채움.

            find_adress_to_addName(print_add, m_name_add, print, &find);//메모리 구조체 배열에 저장된 메모리 이름(add_name)과 print_add를 비교, 일치하는 주소 이름을 찾아 주소값(address)를 찾음. 바이너리 코드 문자열(print = "1001 ") 뒷부분에 추가

            put_at_file(print, wfp); //PRINT [주소이름] 문자열을 파일에 저장
        }//------------------------------------------- PRINT -------------------------------------------


        if (!strncmp("AD", mcode, 2)) { //ADD _, _ 의 앞 2글자 보고 구분
            char first[2]="\0"; //원래 코드로부터 주소 이름1 추출해서 넣을 문자열(first) 만들기(ADD "_", _)
            strncpy(first, mcode + 4, 1);//원래 코드로부터 주소 이름1 추출, load_fir에 넣기

            char load_fir[10] = "0001 "; //LOAD [주소이름1] 바이너리 코드를 만들 문자열. 우선 LOAD를 가리키는 "0001 "(공백 포함)부터 채움.
            char load_num1[10] = "0000 "; //LOAD NUM 바이너리 코드를 만들 문자열. 우선 LOAD를 가리키는 "0000 "(공백 포함)부터 채움.

            find_adress_to_addName(first, m_name_add, load_fir, &find); //메모리 구조체 배열에 저장된 메모리 이름(add_name)과 first를 비교, 일치하는 주소 이름을 찾아 주소값(address)를 찾음. 바이너리 코드 문자열(load_fir = "0001 ") 뒷부분에 추가
            load_add_or_num(&find, first, load_fir, load_num1, wfp); //LOAD [주소 이름1] or LOAD NUM 문자열을 파일에 저장

            put_at_file("0100", wfp);//PUSH 문자열을 파일에 저장


            char second[2] = "\0"; //원래 코드로부터 주소 이름2 추출해서 넣을 문자열(first) 만들기(ADD _, "_")
            strncpy(second, mcode + 7, 1); //원래 코드로부터 주소 이름2 추출, load_sec에 넣기

            char load_sec[10] = "0001 "; //LOAD [주소이름2] 바이너리 코드를 만들 문자열. 우선 LOAD를 가리키는 "0001 "(공백 포함)부터 채움.
            char load_num2[10] = "0000 "; //LOAD NUM 바이너리 코드를 만들 문자열. 우선 LOAD를 가리키는 "0000 "(공백 포함)부터 채움.

            find_adress_to_addName(second, m_name_add, load_sec, &find); //메모리 구조체 배열에 저장된 메모리 이름(add_name)과 second를 비교, 일치하는 주소 이름을 찾아 주소값(address)를 찾음. 바이너리 코드 문자열(load_sec = "0001 ") 뒷부분에 추가
            load_add_or_num(&find, second, load_sec, load_num2, wfp); //LOAD [주소 이름2] or LOAD NUM 문자열을 파일에 저장


            put_at_file("0110", wfp);//ADD 문자열을 파일에 저장


            put_at_file("0101", wfp);//POP 문자열을 파일에 저장
        }//------------------------------------------- ADD -------------------------------------------

        if (!strncmp("SU", mcode, 2)) { //SUB _, _ 의 앞 2글자 보고 구분
            char first[2] = "\0"; //원래 코드로부터 주소 이름1 추출해서 넣을 문자열(first) 만들기(ADD "_", _)
            strncpy(first, mcode + 4, 1);//원래 코드로부터 주소 이름1 추출, load_fir에 넣기

            char load_fir[10] = "0001 "; //LOAD [주소이름1] 바이너리 코드를 만들 문자열. 우선 LOAD를 가리키는 "0001 "(공백 포함)부터 채움.
            char load_num1[10] = "0000 "; //LOAD NUM 바이너리 코드를 만들 문자열. 우선 LOAD를 가리키는 "0000 "(공백 포함)부터 채움.

            find_adress_to_addName(first, m_name_add, load_fir, &find); //메모리 구조체 배열에 저장된 메모리 이름(add_name)과 first를 비교, 일치하는 주소 이름을 찾아 주소값(address)를 찾음. 바이너리 코드 문자열(load_fir = "0001 ") 뒷부분에 추가
            load_add_or_num(&find, first, load_fir, load_num1, wfp); //LOAD [주소 이름1] or LOAD NUM 문자열을 파일에 저장


            put_at_file("0100", wfp);//PUSH 문자열을 파일에 저장


            char second[2] = "\0"; //원래 코드로부터 주소 이름2 추출해서 넣을 문자열(first) 만들기(ADD _, "_")
            strncpy(second, mcode + 7, 1); //원래 코드로부터 주소 이름2 추출, load_sec에 넣기

            char load_sec[10] = "0001 "; //LOAD [주소이름2] 바이너리 코드를 만들 문자열. 우선 LOAD를 가리키는 "0001 "(공백 포함)부터 채움.
            char load_num2[10] = "0000 "; //LOAD NUM 바이너리 코드를 만들 문자열. 우선 LOAD를 가리키는 "0000 "(공백 포함)부터 채움.

            find_adress_to_addName(second, m_name_add, load_sec, &find); //메모리 구조체 배열에 저장된 메모리 이름(add_name)과 second를 비교, 일치하는 주소 이름을 찾아 주소값(address)를 찾음. 바이너리 코드 문자열(load_sec = "0001 ") 뒷부분에 추가
            
            load_add_or_num(&find, second, load_sec, load_num2, wfp); //LOAD [주소 이름2] or LOAD NUM 문자열을 파일에 저장


            put_at_file("0111", wfp);//SUB 문자열을 파일에 저장


            put_at_file("0101", wfp);//POP 문자열을 파일에 저장
        }//SUB
    }

    fclose(rfp); //읽는 파일(프로그램 언어) 닫기
    fclose(wfp); //쓴 파일(바이너리 코드) 닫기

    return 0;
}