#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//���ڿ� �������� \0 ��������

typedef struct memory {
    char add_name[2];
    char addess[5];
} MEMORY; //�޸� ����ü. �޸� �ּ� �̸�, �ּ�(�̸��� ���� �ּҰ��� �����Ű�� ���� [ex] "A = 0111")


void put_at_file(char* A, FILE* B) {
    fputs(A, B);
    fputs("\n", B);
} //���Ͽ� ���̳ʸ� �ڵ� ���ڿ��� �����ϴ� �Լ�

void find_adress_to_addName(char* A, MEMORY* B, char* C, int *find) {
    for (int i = 0; i < 10; i++) {
        if (!strncmp(A, B[i].add_name, 1)) {
            strcat(C, B[i].addess);
            *find = 1;
            break;
        }
    }
}
//"�޸� ����ü �迭(B)�� ����� �޸� �̸�(add_name)"�� "�ڵ�(���α׷� ��� 1��)�κ��� ������ �޸� �̸�(A)"�� ��
//�޸� ����ü �迭���� ��ġ�ϴ� �ּ� �̸��� ã�� �ּҰ�(address)�� ã��. ���̳ʸ� �ڵ� ���ڿ�(C) �޺κп� �߰�

void load_add_or_num(int *find, char *add, char *load_add, char *load_num, FILE* A) {
    if (*find == 1) put_at_file(load_add, A); //LOAD [�ּ� �̸�] ���ڿ��� ���Ͽ� ����
    else if (*find == 0) {
        //printf("%s\n", add);
        long num = strtol(add, NULL, 10); //add(�ּҰ� �ƴ� ������ �Ǹ� -> num)�� 2������ ��ȯ
        char num_str[5];
        ltoa(num, num_str, 2); //2������ ��ȯ�� ���ڸ� num_str ���ڿ��� ����

        char num_4bit[5] = "0000"; //4��Ʈ�� ��ȯ�� num�� ���� ���ڿ�
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
        } //4��Ʈ�� ��ȯ

        strcat(load_num, num_4bit); //LOAD NUM ���̳ʸ� �ڵ� ���ڿ�(load_num) �ڿ� num�� �ٿ��ֱ�
        put_at_file(load_num, A); //LOAD NUM ���̳ʸ� �ڵ� ���Ͽ� �ֱ�
    }
    *find = 0;
} //LOAD [ ]�� LOAD NUM�� �����ϱ� ���� �Լ�
// ���� �޸� �̸�(add)�� ã�Ҵٸ� LOAD [ ] ��ɾ� ���Ͽ� �߰�
// ���� �޸� �̸�(add)�� ã�� ���ߴٸ� add�� long�� ����(2����)�� ��ȯ(num����), 2������ �ٱ� long�� ������ ���ڿ��� ��ȯ, LOAD NUM ��ɾ� ����� ���Ͽ� �߰�

int main() {
    FILE* rfp;//���� ����(���α׷� ���) ���� ������ ����
    FILE* wfp;//�� ����(���̳ʸ� �ڵ�) ���� ������ ����
    rfp = fopen("code.pl", "r");//�ڵ� ����(���α׷� ���) ����
    wfp = fopen("data.bi", "w");//�� ����(���̳ʸ� �ڵ�) ����


    char mcode[20];//��ɾ�(���α׷� ���) ���ڿ��� ���� ����(���� ��ü)

    char add[5] = "\0";//��ɾ�(���α׷� ���)���� �ּ� ������ ���� ����
    char num[5] = "\0";//��ɾ�(���α׷� ���)���� �� ������ ���� ����

    MEMORY m_name_add[10] = {0};//�޸� ����ü �迭(A, B, C... ���� ���� �޸�)
    int index = 0; //�޸� ����ü �迭�� �ε���

    int *find = 0; //find_adress_to_addName �Լ����� �޸𸮸� ã���� ���(1), ã�� ������ ���(0)�� �������ֱ� ���� ����


    if (rfp == NULL) {
        printf("���� file.txt�� �� �� �����ϴ�.\n");
        exit(0);
    }

    while (fgets(mcode, sizeof(mcode), rfp) != NULL)//���� ��� �о���̱�. (mcode�� ���� ��ɾ� ���ڿ� �� ���� NULL�� �ƴϸ�)
    {
        if (!strncmp("WR", mcode, 2)) { //WRITE _, ____, ____ �� �� 2���� ���� ����

            strncpy(m_name_add[index].add_name, mcode + 6, 1);//����ü �迭[index]�� �ּ� �̸�(add_name) �߰� (WRITE "_", ____, ____)

            strncpy(num, mcode + 15, 4);//���� �ڵ�κ��� ��(num) ���� (WRITE _, ____, "____")

            strncpy(add, mcode + 9, 4);//���� �ڵ�κ��� �ּ�(add) ����(WRITE _, "____", ____)
            strncpy(m_name_add[index].addess, add, 4);//����ü �迭[index]�� ������ �ּ� ��(add = addess) �߰�


            char push_n[10] = "0011 "; //PUSH NUM ���̳ʸ� �ڵ带 ���� ���ڿ�. �켱 PUSH�� ����Ű�� "0011 "(���� ����)���� ä��.
            strcat(push_n, num); //���ڿ��� "0011 " ������ ���� �ڵ�κ��� ������ ��(num)�� �־ PUSH NUM ��ɾ �ϼ���Ŵ

            char write[10] = "1000 "; //WRITE Address ���̳ʸ� �ڵ带 ���� ���ڿ�. �켱 WRITE�� ����Ű�� "1000 "(���� ����)���� ä��.
            strcat(write, add); //���ڿ��� "1000 " ������ ���� �ڵ�κ��� ������ �ּ�(add)�� �־� WRITE Address ��ɾ �ϼ���Ŵ

            put_at_file(push_n, wfp);//PUSH NUM ���ڿ��� ���Ͽ� ���� (����� �����Լ� put_at_file ��� <- �� �Ʒ� ����)
            put_at_file(write, wfp);//WRITE Address ���ڿ��� ���Ͽ� ����
            put_at_file("0101", wfp);//POP ���ڿ��� ���Ͽ� ����

            index++;//����ü �迭 index 1 ����
        }//------------------------------------------- WRITE -------------------------------------------


        if (!strncmp("ST", mcode, 2)) { //STORE _ �� �� 2���� ���� ����
            char store_add[2] = "\0"; //���� �ڵ�κ��� �ּ� �̸� �����ؼ� ���� ���ڿ�(store_add) �����(STORE "_")
            strncpy(store_add, mcode + 6, 1); //���� �ڵ�κ��� �ּ� �̸� ����, store_add�� �ֱ�

            char store[10] = "0010 "; //STORE [�ּ��̸�] ���̳ʸ� �ڵ带 ���� ���ڿ�. �켱 STORE�� ����Ű�� "0010 "(���� ����)���� ä��.

            find_adress_to_addName(store_add, m_name_add, store, &find); //�޸� ����ü �迭�� ����� �޸� �̸�(add_name)�� store_add�� ��, ��ġ�ϴ� �ּ� �̸��� ã�� �ּҰ�(address)�� ã��. ���̳ʸ� �ڵ� ���ڿ�(store = "0010 ") �޺κп� �߰�

            put_at_file(store, wfp); //STORE [�ּ��̸�] ���ڿ��� ���Ͽ� ����
        }//------------------------------------------- STORE -------------------------------------------


        if (!strncmp("PR", mcode, 2)) { //PRINT _ �� �� 2���� ���� ����
            char print_add[2] = "\0"; //���� �ڵ�κ��� �ּ� �̸� �����ؼ� ���� ���ڿ�(print_add) �����(PRINT "_")
            strncpy(print_add, mcode + 6, 1); //���� �ڵ�κ��� �ּ� �̸� ����, print_add�� �ֱ�

            char print[10] = "1001 "; //PRINT [�ּ��̸�] ���̳ʸ� �ڵ带 ���� ���ڿ�. �켱 PRINT�� ����Ű�� "1001 "(���� ����)���� ä��.

            find_adress_to_addName(print_add, m_name_add, print, &find);//�޸� ����ü �迭�� ����� �޸� �̸�(add_name)�� print_add�� ��, ��ġ�ϴ� �ּ� �̸��� ã�� �ּҰ�(address)�� ã��. ���̳ʸ� �ڵ� ���ڿ�(print = "1001 ") �޺κп� �߰�

            put_at_file(print, wfp); //PRINT [�ּ��̸�] ���ڿ��� ���Ͽ� ����
        }//------------------------------------------- PRINT -------------------------------------------


        if (!strncmp("AD", mcode, 2)) { //ADD _, _ �� �� 2���� ���� ����
            char first[2]="\0"; //���� �ڵ�κ��� �ּ� �̸�1 �����ؼ� ���� ���ڿ�(first) �����(ADD "_", _)
            strncpy(first, mcode + 4, 1);//���� �ڵ�κ��� �ּ� �̸�1 ����, load_fir�� �ֱ�

            char load_fir[10] = "0001 "; //LOAD [�ּ��̸�1] ���̳ʸ� �ڵ带 ���� ���ڿ�. �켱 LOAD�� ����Ű�� "0001 "(���� ����)���� ä��.
            char load_num1[10] = "0000 "; //LOAD NUM ���̳ʸ� �ڵ带 ���� ���ڿ�. �켱 LOAD�� ����Ű�� "0000 "(���� ����)���� ä��.

            find_adress_to_addName(first, m_name_add, load_fir, &find); //�޸� ����ü �迭�� ����� �޸� �̸�(add_name)�� first�� ��, ��ġ�ϴ� �ּ� �̸��� ã�� �ּҰ�(address)�� ã��. ���̳ʸ� �ڵ� ���ڿ�(load_fir = "0001 ") �޺κп� �߰�
            load_add_or_num(&find, first, load_fir, load_num1, wfp); //LOAD [�ּ� �̸�1] or LOAD NUM ���ڿ��� ���Ͽ� ����

            put_at_file("0100", wfp);//PUSH ���ڿ��� ���Ͽ� ����


            char second[2] = "\0"; //���� �ڵ�κ��� �ּ� �̸�2 �����ؼ� ���� ���ڿ�(first) �����(ADD _, "_")
            strncpy(second, mcode + 7, 1); //���� �ڵ�κ��� �ּ� �̸�2 ����, load_sec�� �ֱ�

            char load_sec[10] = "0001 "; //LOAD [�ּ��̸�2] ���̳ʸ� �ڵ带 ���� ���ڿ�. �켱 LOAD�� ����Ű�� "0001 "(���� ����)���� ä��.
            char load_num2[10] = "0000 "; //LOAD NUM ���̳ʸ� �ڵ带 ���� ���ڿ�. �켱 LOAD�� ����Ű�� "0000 "(���� ����)���� ä��.

            find_adress_to_addName(second, m_name_add, load_sec, &find); //�޸� ����ü �迭�� ����� �޸� �̸�(add_name)�� second�� ��, ��ġ�ϴ� �ּ� �̸��� ã�� �ּҰ�(address)�� ã��. ���̳ʸ� �ڵ� ���ڿ�(load_sec = "0001 ") �޺κп� �߰�
            load_add_or_num(&find, second, load_sec, load_num2, wfp); //LOAD [�ּ� �̸�2] or LOAD NUM ���ڿ��� ���Ͽ� ����


            put_at_file("0110", wfp);//ADD ���ڿ��� ���Ͽ� ����


            put_at_file("0101", wfp);//POP ���ڿ��� ���Ͽ� ����
        }//------------------------------------------- ADD -------------------------------------------

        if (!strncmp("SU", mcode, 2)) { //SUB _, _ �� �� 2���� ���� ����
            char first[2] = "\0"; //���� �ڵ�κ��� �ּ� �̸�1 �����ؼ� ���� ���ڿ�(first) �����(ADD "_", _)
            strncpy(first, mcode + 4, 1);//���� �ڵ�κ��� �ּ� �̸�1 ����, load_fir�� �ֱ�

            char load_fir[10] = "0001 "; //LOAD [�ּ��̸�1] ���̳ʸ� �ڵ带 ���� ���ڿ�. �켱 LOAD�� ����Ű�� "0001 "(���� ����)���� ä��.
            char load_num1[10] = "0000 "; //LOAD NUM ���̳ʸ� �ڵ带 ���� ���ڿ�. �켱 LOAD�� ����Ű�� "0000 "(���� ����)���� ä��.

            find_adress_to_addName(first, m_name_add, load_fir, &find); //�޸� ����ü �迭�� ����� �޸� �̸�(add_name)�� first�� ��, ��ġ�ϴ� �ּ� �̸��� ã�� �ּҰ�(address)�� ã��. ���̳ʸ� �ڵ� ���ڿ�(load_fir = "0001 ") �޺κп� �߰�
            load_add_or_num(&find, first, load_fir, load_num1, wfp); //LOAD [�ּ� �̸�1] or LOAD NUM ���ڿ��� ���Ͽ� ����


            put_at_file("0100", wfp);//PUSH ���ڿ��� ���Ͽ� ����


            char second[2] = "\0"; //���� �ڵ�κ��� �ּ� �̸�2 �����ؼ� ���� ���ڿ�(first) �����(ADD _, "_")
            strncpy(second, mcode + 7, 1); //���� �ڵ�κ��� �ּ� �̸�2 ����, load_sec�� �ֱ�

            char load_sec[10] = "0001 "; //LOAD [�ּ��̸�2] ���̳ʸ� �ڵ带 ���� ���ڿ�. �켱 LOAD�� ����Ű�� "0001 "(���� ����)���� ä��.
            char load_num2[10] = "0000 "; //LOAD NUM ���̳ʸ� �ڵ带 ���� ���ڿ�. �켱 LOAD�� ����Ű�� "0000 "(���� ����)���� ä��.

            find_adress_to_addName(second, m_name_add, load_sec, &find); //�޸� ����ü �迭�� ����� �޸� �̸�(add_name)�� second�� ��, ��ġ�ϴ� �ּ� �̸��� ã�� �ּҰ�(address)�� ã��. ���̳ʸ� �ڵ� ���ڿ�(load_sec = "0001 ") �޺κп� �߰�
            
            load_add_or_num(&find, second, load_sec, load_num2, wfp); //LOAD [�ּ� �̸�2] or LOAD NUM ���ڿ��� ���Ͽ� ����


            put_at_file("0111", wfp);//SUB ���ڿ��� ���Ͽ� ����


            put_at_file("0101", wfp);//POP ���ڿ��� ���Ͽ� ����
        }//SUB
    }

    fclose(rfp); //�д� ����(���α׷� ���) �ݱ�
    fclose(wfp); //�� ����(���̳ʸ� �ڵ�) �ݱ�

    return 0;
}