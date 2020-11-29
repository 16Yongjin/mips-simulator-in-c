// 구조체 비트 필드 사용하기. 참고: https://dojang.io/mod/page/view.php?id=472
union IR {
  unsigned int B;

  struct RFormat {
    unsigned int funct: 6;
    unsigned int sh: 5;
    unsigned int rd: 5;
    unsigned int rt: 5;
    unsigned int rs: 5;
    unsigned int opcode: 6; 
  } R;

  struct IFormat {
    int operand: 16;
    unsigned int rt: 5;
    unsigned int rs: 5;
    unsigned int op: 6;
  } I;

  struct JFormat {
    unsigned int address: 26;
    unsigned int op: 6;
  } J;
};

// OPCODE
enum OPCODE {
  R_FORMAT = 0,
  BLTZ = 1,
  J = 2,
  JAL = 3,
  BEQ = 4,
  BNE = 5,
  ADDI = 8,
  ADDIU = 9,
  SLTI = 10,
  SLTIU = 11,
  ANDI = 12,
  ORI = 13,
  XORI = 14,
  LUI = 15,
  LB = 32,
  LW = 35,
  LBU = 36,
  SB = 40,
  SW = 43
};

// FUNCT
enum FUNCT {
  SLL = 0,
  SRL = 2,
  SRA = 3,
  JR = 8,
  SYSCALL = 12,
  MFHI = 16,
  MFLO = 18,
  MUL = 24,
  DIV = 26,
  ADD = 32,
  SUB = 34,
  AND = 36,
  OR,
  XOR,
  NOR,
  SLT = 42
};

// 명령어 8 x 6 배열을 1차원으로 폈음
static char OPCODE_STR[48][10] = {
  "R-format", "bltz", "j", "jal", "beq", "bne", "", "", "addi", "", "slti", "", "andi", "ori", "xori", "lui", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "lb", "", "", "lw", "lbu", "", "", "", "sb", "", "", "sw", "", "", "", ""
};

static char FUNCT_STR[48][10] = {
  "sll", "", "srl", "sra", "", "", "", "", "jr", "", "", "", "syscall", "", "", "", "", "mfhi", "", "mflo", "", "", "", "", "", "mul", "", "", "", "", "", "", "add", "", "sub", "", "and", "or", "xor", "nor", "", "", "slt", "", "", "", "", ""
};

// 명령어 워드를 받으면 문자열로 포맷팅합니다.
void instructionToString(unsigned int word, char** str);

// 명령어 워드를 받으면 문자열로 포맷팅 후 출력합니다.
void printInstruction(unsigned int word);