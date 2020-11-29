#include "memory.h"
#include "register.h"
#include "decode.h"
#include "utils.h"
#include "alu.h"
#include "utils.h"

// register.c에 있는 PC와 HI, LO 값을 참조함
extern int PC;
extern int HI;
extern int LO;

// register.c의 REGISTERS의 alias인 R
// REG 함수 쓰기 귀찮아서 R 배열로 접근합니다.
extern unsigned int* R;

/**
 [구현 방법]

 함수 내부 주석 대로 명령어를 구현합니다.

 명령어 실행 중 문제 발생 시 1을 반환하고
 명령어 실행이 정상적으로 끝나면 0을 반환합니다.

 레지스터 접근: 포인터 R 사용. ex) R[rd]
 메모리 접근: MEM 함수 사용. (※ READ_MEMORY ENUM이 -> READ로 변경됨)
*/

/** R 포맷 명령어 */

int sll(int rd, int rt, int sh) {
  // R[$rd] ← R[$rt] << shamt
  R[rd] = ALU(R[rt], sh, ALU_SLL, NULL);
  return 0;
}

int srl(int rd, int rt, int sh) {
  // R[$rd] ← R[$rt] >> shamt
  R[rd] = ALU(R[rt], sh, ALU_SRL, NULL);
  return 0;
}

int sra(int rd, int rt, int sh) {
  // R[$rd] ← R[$rt] >> shamt
  R[rd] = ALU(R[rt], sh, ALU_SRA, NULL);
  return 0;
}

int jr(int rs) {
  // PC ← R[$rs]
  PC = R[rs];
  return 0;
}


// Syscall 테이블 참고: https://bit.ly/3k9Rmfl
int syscall(){
  switch (R[$v0]) {
    case 1:  printf("%d\n", R[$a0]); break;
    case 10: printf("exit program\n"); return 1;
    case 11: printf("%c\n", R[$a0]); break;
  }
  return 0;
}

int mfhi(int rd){
  // R[$rd] ← HI
  R[rd] = HI;
  return 0;
}

int mflo(int rd){
  // R[$rd] ← LO
  R[rd] = LO;
  return 0;
}

int mul(int rs, int rt){
  // {HI, LO} ← R[$rs] * R[$rt]
  long long int result = (long long int)R[rs] * (long long int)R[rt];
  HI = result >> 32;
  LO = (result << 32) >> 32;

  return 0;
}

int add(int rd, int rs, int rt){
  // R[$rd] ← R[$rs] + R[$rt]
  R[rd] = ALU(R[rs], R[rt], ALU_ADD, NULL);
  printDebug("[DEBUG] ADD 결과: R[rd] %x\n", R[rd]);
  return 0;
}

int sub(int rd, int rs, int rt){
  // R[$rd] ← R[$rs] - R[$rt]
  R[rd] = ALU(R[rs], R[rt], ALU_SUBTRACT, NULL);
  printDebug("[DEBUG] SUB 결과: R[rd] %x\n", R[rd]);
  return 0;
}

int and(int rd, int rs, int rt){
  // R[$rd] ← R[$rs] & R[$rt]
  R[rd] = ALU(R[rs], R[rt], ALU_AND, NULL);
  return 0;
}

int or(int rd, int rs, int rt){
  // R[$rd] ← R[$rs] | R[$rt]
    R[rd] = ALU(R[rs], R[rt], ALU_OR, NULL);
  return 0;
}

int xor(int rd, int rs, int rt){
  // R[$rd] ← R[$rs] ^ R[$rt]
  R[rd] = ALU(R[rs], R[rt], ALU_XOR, NULL);
  return 0;
}

int nor(int rd, int rs, int rt){
  R[rd] = ALU(R[rs], R[rt], ALU_NOR, NULL);
  return 0;
}

int slt(int rd, int rs, int rt){
  // TODO: [상우님] ALU_SET_LESS 사용
  // R[$rd] ← R[$rs] < R[$rt]
  R[rd] = ALU(R[rs], R[rt], ALU_SET_LESS, NULL);
  return 0;
}


/** J 포맷 명령어 */

int j(int address){
  // PC ← {(PC + 4)[31:28], address, 00}
  PC = ((PC + 4) & 0xf0000000) | (address << 2);
  return 0;
}

int jal(int address){
  // R[31] ← PC + 8
  // PC ← {(PC + 4)[31:28], address, 00}
  R[$ra] = PC;
  PC = ((PC + 4) & 0xf0000000) | (address << 2);
  return 0;
}

/** I 포맷 명령어 */

int bltz(int rs, int rt, int imm){
  if (R[rs] < R[rt]) PC += imm * 4;
  return 0;
}

int beq(int rs, int rt, int imm){
  // if(R[$rs] = R[$rt])
  //   PC ← PC + 4 + SignExt18b({imm, 00})
  if (R[rs] == R[rt]) PC += imm * 4;
  return 0;
}

int bne(int rs, int rt, int imm){
  // if(R[$rs] != R[$rt])
  //  PC ← PC + 4 + SignExt18b({imm, 00})
  if (R[rs] != R[rt]) PC += imm * 4;
  return 0;
}

int addi(int rt, int rs, int imm){
  // R[$rt] ← R[$rs] + SignExt16b(imm)
  R[rt] = ALU(R[rs], imm, ALU_ADD, NULL);
  return 0;
}

int slti(int rt, int rs, int imm){
  // R[$rt] ← R[$rs] < SignExt16b(imm)
  R[rt] = ALU(R[rs], imm, ALU_SET_LESS, NULL);
  return 0;
}

int andi(int rt, int rs, int imm){
  // R[$rt] ← R[$rs] & {0 × 16, imm}
  R[rt] = ALU(R[rs], imm, ALU_AND, NULL);
  return 0;
}


int ori(int rt, int rs, int imm){
  // R[$rt] ← R[$rs] | {0 × 16, imm}
  R[rt] = ALU(R[rs], imm, ALU_OR, NULL);
  return 0;
}

int xori(int rt, int rs, int imm){
  // R[$rt] ← R[$rs] ^ {0 × 16, imm}
  R[rt] = ALU(R[rs], imm, ALU_XOR, NULL);
  return 0;
}

int lui(int rt, int imm) {
  // R[$rt] ← {(imm)[15:0], 0 × 16}
  R[rt] = (imm & 0xffff) << 16;
  return 0;
}

int lb(int rt, int imm, int rs){
  // R[$rt] ← SignExt8b(Mem1B(R[$rs] + SignExt16b(imm)))
  R[rt] = MEM(R[rs] + imm, 0, READ, BYTE);
  printDebug("[DEBUG] lb 읽은 메모리: 0x%x\n", R[rt]);
  return 0;
}

int lw(int rt, int imm, int rs){
  // R[$rt] ← Mem4B(R[$rs] + SignExt16b(imm))
  R[rt] = MEM(R[rs] + imm, 0, READ, WORD);
  printDebug("[DEBUG] lw 읽은 메모리: 0x%x\n", R[rt]);
  return 0;
}

int lbu(int rt, int imm, int rs){
  // R[$rt] ← {0 × 24, Mem1B(R[$rs] + SignExt16b(imm))}
  R[rt] = MEM(R[rs] + imm, 0, READ, BYTE);
  return 0;
}

int sb(int rt, int imm, int rs){
  // Mem1B(R[$rs] + SignExt16b(imm)) ← (R[$rt])[7:0]
  MEM(R[rs] + imm, R[rt], WRITE, BYTE);
  return 0;
}

int sw(int rt, int imm, int rs){
  // Mem4B(R[$rs] + SignExt16b(imm)) ← R[$rt]
  MEM(imm + R[rs], R[rt], WRITE, WORD);
  return 0;
}


union IR instructionFetch() {
   // 1. PC 값을 읽어서 메모리에서 명령어 가져오기
  unsigned int word = MEM(PC, 0, READ, WORD);
  union IR instruction = (union IR)word;
  // 2. PC 값 4 상승
  PC += 4;

  return instruction;
}

// 명령어 한 단계 씩 실행. s 명령어 입력 시 실행됨
int stepProgram() {
  // TODO: 명렁어 하나만 실행
  // 1. Instruction Fetch: 명령어 가져오기
  union IR instruction = instructionFetch();

  printDebug("[DEBUG] 실행한 명령어: ");
  printInstruction(instruction.B);

  // 임시로 명령어가 0x00000000이면 실행 중지
  // 0x00000000인 명령어도 있지만 (sll 0 0 0)
  // go 명령어 실행 시 멈출 방법이 없기 때문
  if (instruction.B == 0) return 1;

  // 2. Instruction Decode: 명령어 해석
  unsigned int opcode = instruction.R.opcode;
  unsigned int funct = instruction.R.funct;
  unsigned int rs = instruction.R.rs;
  unsigned int rt = instruction.R.rt;
  unsigned int rd = instruction.R.rd;
  unsigned int sh = instruction.R.sh;
  unsigned int address = instruction.J.address;
  unsigned int u_imm = instruction.I.operand & 0xffff;
  int imm = instruction.I.operand; // Sign Extendted

  // 3. EX, MA, RW: 명령어를 분기하고 해당되는 함수 실행
  if (opcode == R_FORMAT) {
    switch (funct) {
      case SLL:     return sll(rd, rt, sh);
      case SRL:     return srl(rd, rt, sh);
      case SRA:     return sra(rd, rt, sh);
      case JR:      return jr(rs);
      case SYSCALL: return syscall();
      case MFHI:    return mfhi(rd);
      case MFLO:    return mflo(rd);
      case MUL:     return mul(rs, rt);
      case ADD:     return add(rd, rs, rt);
      case SUB:     return sub(rd, rs, rt);
      case AND:     return and(rd, rs, rt);
      case OR:      return or(rd, rs, rt);
      case XOR:     return xor(rd, rs, rt);
      case NOR:     return nor(rd, rs, rt);
      case SLT:     return slt(rd, rs, rt);
      default:      printError("[ERROR] unknown instruction\n");
    }
  } else {
    switch (opcode) {
      case J:    return j(address);
      case JAL:  return jal(address);
      case BLTZ: return bltz(rs, rt, imm);
      case BEQ:  return beq(rs, rt, imm); 
      case BNE:  return bne(rs, rt, imm);
      case ADDI: return addi(rt, rs, imm);
      case SLTI: return slti(rt, rs, imm);
      case ANDI: return andi(rt, rs, u_imm);
      case ORI:  return ori(rt, rs, u_imm);
      case XORI: return xori(rt, rs, u_imm);
      case LUI:  return lui(rt, u_imm);
      case LW:   return lw(rt, imm, rs);
      case SW:   return sw(rt, imm, rs);
      case LB:   return lb(rt, imm, rs);
      case SB:   return sb(rt, imm, rs);
      case LBU:  return lbu(rt, imm, rs);
      default:   printError("[ERROR] unknown instruction\n");
    }
  }

  return 1; // 프로그램 끝났으면 1 반환, 아니면 0 반환
}

// 프로그램 전체 실행. g 명령어 입력 시 실행됨
void goProgram() {
  // 프로그램 syscall 10까지 실행
  while (1) {
    int end = stepProgram();
    if (end) return;
  }
}
