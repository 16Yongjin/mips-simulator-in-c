#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "memory.h"
#include "utils.h"

#define MEMORY_SIZE 0x100000

unsigned char progMEM[MEMORY_SIZE], dataMEM[MEMORY_SIZE], stackMEM[MEMORY_SIZE];

int MEM(unsigned int A, int V, int nRW, int S) {
  unsigned int sel = A >> 20;
  unsigned int offset = A & 0xFFFFF;
  unsigned char *pM;

  // 메모리 종류 선택
  if (sel == 0x004) pM = progMEM;         // program memory
  else if (sel == 0x100) pM = dataMEM;  // data memory
  else if (sel == 0x7FF) pM = stackMEM;  // stack
  else { printError("[ERROR] 잘못된 메모리 접근\n"); return -1; }

  // 워드 사이즈 선택
  int size = 0;
  if (S == BYTE) size = 1;
  else if (S == HALF_WORD) size = 2;
  else if (S == WORD) size = 4;

  if (nRW == 0) { // 메모리 읽기
    int word = 0;

    for (int i = 0; i < size; i++)
      ((unsigned char*)&word)[size - i - 1] = pM[offset + i];

    return word;
  } else if (nRW == WRITE) { // 메모리 쓰기
    for (int i = 0; i < size; i++) 
      pM[offset + (size - i - 1)] = ((unsigned char*)&V)[i];
  }

  return 0;
}

// 메모리 보기. m 명령어 입력 시 실행됨
void viewMemory(unsigned int start, unsigned int end) {
  printf("\033[0;34m"); // 파란색 글씨
  printf("------------------------\n");
  printf("|  address |   value   |\n");
  printf("|----------------------|\n");
  for (int address = start; address <= end; address += 4) {
    // 각 주소에 해당하는 메모리를 워드 단위로 읽어서 출력하기
    int word = MEM(address, 0, READ, WORD);
    printf("| %8x | %9x |\n", address, word);
  }
  printf("------------------------\n");
  printf("\033[0m");
}

// 프로그램, 데이터, 스택 메모리 값 모두 0으로 설정
void resetMemory() {
  memset(progMEM, 0, sizeof(progMEM));
	memset(stackMEM, 0, sizeof(stackMEM));
	memset(dataMEM, 0, sizeof(dataMEM));
}

// address의 주소를 value 값으로 설정
// sm 명령어 입력 시 실행됨
void setMemory(unsigned int address, int value) {
  MEM(address, value, WRITE, WORD);
}
