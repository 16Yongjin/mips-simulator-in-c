#include <stdio.h>
#include "register.h"

enum READ_WRITE { READ, WRITE };

unsigned int REGISTERS[REGISTER_SIZE];

// REGISTERS가 너무 길어서 대신 사용할 alias
unsigned int* R = REGISTERS;

int PC = 0x400000;
int HI = 0;
int LO = 0;

// 레지스터 접근 인터페이스
unsigned int REG(unsigned int A, unsigned int V, unsigned int nRW) {
  if (nRW == READ) return REGISTERS[A];
  else if (nRW == WRITE) REGISTERS[A] = V;
  return 0;
}

// 레지스터 보기. r 명령어 입력 시 실행됨
void viewRegister() {
  printf("\033[0;32m"); // 초록색 글씨
  printf("------------------------\n");
  printf("| register |   value   |\n");
  printf("|----------------------|\n");
  printf("| %8s | %9x |\n", "PC", PC);
  printf("| %8s | %9x |\n", "HI", HI);
  printf("| %8s | %9x |\n", "LO", LO);
  printf("|          |           |\n");
  for (int i = 0; i < REGISTER_SIZE; i++) {
    // 각각의 레지스터 읽어오기
    char* name = REGISTER_STR[i];
    unsigned int value = REG(i, 0, READ);
    printf("| %8s | %9x |\n", name, value);
  }
   printf("------------------------\n");
   printf("\033[0m");
}

// 각각의 레지스터의 값을 0으로 설정함
void resetRegister() {
  for (int i = 0; i < REGISTER_SIZE; i++) {
    REG(i, 0, WRITE);
  }

  HI = 0;
  LO = 0;
  REG($sp, 0x80000000, WRITE);
}

// 레지스터 값 설정. sr 명령어 입력 시 실행됨
void setRegister(unsigned int number, int value) {
  REG(number, value, WRITE);
}

// 프로그램 점프하기. j 명령어 입력 시 실행됨
void jumpProgram(unsigned int address) {
  // PC를 address로 설정
  PC = address;
}
