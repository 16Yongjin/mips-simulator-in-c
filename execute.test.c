#include "execute.h"
#include "memory.h"
#include "register.h"
#include "decode.h"
#include "utils.h"
#include "alu.h"
#include "utils.h"

// register.c의 REGISTERS
extern unsigned int* R;
extern int PC;
extern int HI;
extern int LO;

// 예상 결과(expect)가 실제 결과(result)와 같은지 확인하고 결과 출력
void printResult(char name[], int expected, int result) {
  if (result == expected) {
    printSuccess("[TEST SUCCESS] %s 테스트 성공\n", name);
  } else {
    printError("[TEST FAIL] %s 테스트 실패: 예상: %x, 실제: %x\n", name, expected, result);
  }
}

// sll, srl, sra 테스트용
// $t1에 rtV 값을 넣고 sh 만큼 쉬프트해서 $t0 레지스터에 저장
// $t0의 값이 원하는 값인지 확인함
void shamtTest(char name[], int (*func)(), int rtV, int sh, unsigned int expected) {
  resetRegister();

  R[$t1] = rtV;
  (*func)($t0, $t1, sh);
  unsigned int result = R[$t0];

  printResult(name, expected, result);
}

// add, sub, and, or 등 테스트용
// $t1에 rsV 값을, $t2에 rtV 값을 넣고 계산한 값을 $t0에 넣음
// $t0의 값이 원하는 값인지 확인함
void aluTest(char name[], int (*func)(), int rsV, int rtV, int expected) {
  resetRegister();

  R[$t1] = rsV;
  R[$t2] = rtV;
  (*func)($t0, $t1, $t2);
  int result = R[$t0];

  printResult(name, expected, result);
}

// addi, andi, ori 등 테스트용
// $t1에 rsV 값을 넣고 이를 imm과 계산한 값을 $t0에 넣음
// $t0의 값이 원하는 값인지 확인함
void immAluTest(char name[], int (*func)(), int rsV, int imm, int expected) {
  resetRegister();

  R[$t1] = rsV;
  (*func)($t0, $t1, imm);
  int result = R[$t0];

  printResult(name, expected, result);
}

void multTest(int rsV, int rtV, long long int expected)  {
  resetRegister();

  R[$t1] = rsV;
  R[$t2] = rtV;
  mul($t1, $t2);

  long long int result = ((long long int)HI << 32) | LO;

  if (result == expected) {
    printSuccess("[TEST SUCCESS] mul 테스트 성공\n");
  } else {
    printError("[TEST FAIL] mul 테스트 실패: 예상: %x, 실제: %x\n", expected, result);
  }
}

// 명령어 실행 테스트
// t 명령어 입력 시 실행됨
void executeTest() {
  printDebug("[명령어 테스트 실행]\n");

  // R 타입 일부 명령어 테스트
  shamtTest("sll", &sll, 0xfff, 4, 0xfff << 4);
  shamtTest("srl", &srl, 0xfff, 4, (unsigned)0xfff >> 4);
  shamtTest("sra", &srl, 0xfff, 4, 0xfff >> 4);

  aluTest("add", &add, 1, 1, 1 + 1);
  aluTest("sub", &sub, 2, 1, 2 - 1);
  aluTest("and", &and, 0x111, 0x11, 0x111 & 0x11);
  aluTest("or", &or, 0x1100, 0x11, 0x1100 | 0x11);
  aluTest("xor", &xor, 0x1100, 0x11, 0x1100 ^ 0x11);
  aluTest("nor", &nor, 0x1100, 0x11, ~(0x1100 | 0x11));
  aluTest("slt", &slt, 1, 2, 1 < 2);

  // I 타입 일부 명령어 테스트
  immAluTest("addi", &addi, 1, 1, 1 + 1);
  immAluTest("andi", &andi, 0x111, 0x11, 0x111 & 0x11);
  immAluTest("ori", &ori, 0x1100, 0x11, 0x1100 | 0x11);
  immAluTest("xori", &xori, 0x1100, 0x11, 0x1100 ^ 0x11);
  immAluTest("slti", &slti, 1, 2, 1 < 2);
  
  multTest(100, 100, 100 * 100);
  long long int mulRes2 = 100000000;
  mulRes2 *= 100000000;
  multTest(100000000, 100000000, mulRes2);
}
