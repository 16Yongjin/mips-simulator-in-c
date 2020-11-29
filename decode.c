#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h"
#include "register.h"

// 명렁어 워드를 파싱해서 문자열로 변환함
void instructionToString(unsigned int word, char** str) {
  // 비트 파싱은 아래 코드가 끝
  union IR instruction = (union IR)word;

  // 각종 데이터 op, fn, rs, rt, rd, sh, im함 등을 다 미리 준비함
  unsigned int opcode = instruction.R.opcode;
  unsigned int funct = instruction.R.funct;
  char * op = opcode ? OPCODE_STR[opcode] : FUNCT_STR[funct];
  char * rs = REGISTER_STR[instruction.R.rs];
  char * rt = REGISTER_STR[instruction.R.rt];
  char * rd = REGISTER_STR[instruction.R.rd];
  unsigned int sh = instruction.R.sh;
  unsigned int target = instruction.J.address;
  unsigned int u_immediate = instruction.I.operand;
  int immediate = (int)instruction.I.operand; // Sign Extendted

  // 열거형 상수를 이용한 switch문. 참고 https://bit.ly/34M0l1P
  // sprintf 사용법 참고: https://bit.ly/320xPrD
  // 그냥 printf를 써도 되지만, 재사용성을 위해 문자열을 포맷 후 str에 저장
  // 포맷된 문자열을 출력하는 함수는 아래의 printInstruction() 사용
  if (opcode == R_FORMAT) {
    switch (funct) {
      case SLL: case SRL: case SRA:
        sprintf(*str, "%s %s, %s, %d", op, rd, rt, sh);
        break;
      case JR:
        sprintf(*str, "%s %s", op, rs);
        break;
      case SYSCALL:
        sprintf(*str, "syscall");
        break;
      case MFHI: case MFLO:
        sprintf(*str, "%s %s", op, rd);
        break;
      default:
        sprintf(*str, "%s %s, %s, %s", op, rd, rs, rt);
    }
  } else {
    switch (opcode) {
      case J: case JAL:
        sprintf(*str, "%s %x", op, target);
        break;
      case BLTZ:
        sprintf(*str, "%s %s, %d", op, rs, immediate);
        break;
      case BEQ: case BNE:
        sprintf(*str, "%s %s, %s, %d", op, rs, rt, immediate);
        break;
      case ADDI: case ADDIU: case SLTI: case SLTIU:
        sprintf(*str, "%s %s, %s, %d", op, rt, rs, immediate);
        break;
      case ANDI: case ORI: case XORI:
        sprintf(*str, "%s %s, %s, %d", op, rt, rs, u_immediate);
        break;
      case LUI:
        sprintf(*str, "%s %s, %d", op, rt, u_immediate);
        break;
      case LW: case SW: case LB: case SB: case LBU:
        sprintf(*str, "%s %s, %d(%s)", op, rt, immediate, rs);
        break;
      default:
        sprintf(*str, "Unknown Instruction: %x", instruction.B);
    }
  }
}

// 명령어 출력하기
void printInstruction(unsigned int word) {
  char* str = (char*)malloc((100) * sizeof(char));
  instructionToString(word, &str);
  printf("%s\n", str);
  free(str);
}
