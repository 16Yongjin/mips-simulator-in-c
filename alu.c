#include <stdio.h>
#include <stdlib.h>
#include "alu.h"
#include "utils.h"

// 열거형 상수를 이용한 switch문 참고 https://bit.ly/34M0l1P

enum SHIFT_OPERATION { NO_SHIFT, SLL, SRL, SRA };

enum ADD_SUB { ADD, SUB };

enum LOGIC_OPERATION { AND, OR, XOR, NOR };

enum ALU_OPERATION { SHIFT, CHECK_ZERO, ADD_SUB, LOGIC_OPERATION };

char* ALU_OPERATION_STR[16] = { "No Shift", "SLL", "SRL", "SRA", "Set Less", "Set Less", "Set Less", "Set Less", "Add", "Subtract", "Add", "Subtract", "AND", "OR", "XOR", "NOR" };

int logicOperation(int X, int Y, int S) {
  switch (S) {
    case AND: return X & Y;
    case OR: return X | Y;
    case XOR: return X ^ Y;
    case NOR: return ~(X | Y);
    default: printError("[ERROR] error in logic operation\n"); exit(1);
  }
}

int shiftOperation(int X, int Y, int C) {
  switch (C) {
    case NO_SHIFT: return X;
    case SLL: return X << Y;
    case SRL: return (unsigned)X >> Y;
    case SRA: return X >> Y;
    default: printError("[ERROR] error in shift operation\n"); exit(1);
  }
}

int addSubtract(int X, int Y, int C) {
  switch (C) {
    case ADD: return X + Y;
    case SUB: return X - Y;
    default: printError("[ERROR] error in add/sub operation\n"); exit(1);
  }
}

int isZero(int S) {
  return S == 0;
}

// S로 입력받은 값이 0이면 Z를 1로 설정하고, 입력받은 S 값은 그대로 출력함
int checkZero(int S, int *Z) {
  if (Z != NULL) *Z = isZero(S);
  return S;
}

int checkSetLess(int X, int Y) {
  return X < Y;
}

int ALU(int X, int Y, int S, int *Z) {
  printDebug("[DEBUG] ALU %s 연산 실행\n", ALU_OPERATION_STR[S]);

  int S32 = (S >> 2) & 3;
  int S10 = S & 3;
  int S0 = S & 1;

  switch (S32) {
    case SHIFT: return shiftOperation(X, Y, S10);
    case CHECK_ZERO: return checkSetLess(X, Y);
    case ADD_SUB: return checkZero(addSubtract(X, Y, S0), Z);
    case LOGIC_OPERATION: return logicOperation(X, Y, S10);
    default: printError("[ERROR] Compiler is weired\n"); exit(1);
  }
}
