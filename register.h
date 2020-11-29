#define REGISTER_SIZE 32

enum REGISTER_ENUM {
  $zero, $at, $v0, $v1, $a0, $a1, $a2, $a3, $t0, $t1, $t2, $t3, $t4, $t5, $t6, $g7, $s0, $s1, $s2, $s3, $s4, $s5, $s6, $s7, $t8, $t9, $k0, $k1, $gp, $sp, $fp, $ra
};

static char REGISTER_STR[REGISTER_SIZE][6] = {
  "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$g7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};

// 레지스터 접근 인터페이스
unsigned int REG(unsigned int A, unsigned int V, unsigned int nRW);

// 레지스터 보기. r 명령어 입력 시 실행됨
void viewRegister();

// 각각의 레지스터의 값을 0으로 설정함
void resetRegister();

// 레지스터 값 설정. sr 명령어 입력 시 실행됨
void setRegister(unsigned int number, int value);

// 프로그램 점프하기. j 명령어 입력 시 실행됨
void jumpProgram(unsigned int address);