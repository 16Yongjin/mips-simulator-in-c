// 명령어 테이블 참조: https://bit.ly/3p78Vk4


// 메모리에서 PC가 가리키는 명령어 가져오기
union IR instructionFetch();

// 명령어 한 단계 씩 실행. s 명령어 입력 시 실행됨
int stepProgram();

// 프로그램 전체 실행. g 명령어 입력 시 실행됨
void goProgram();

// R 포맷 명령어
int sll(int rd, int rt, int sh);
int srl(int rd, int rt, int sh);
int sra(int rd, int rt, int sh);
int jr(int rs);
int syscall();
int mfhi(int rd);
int mflo(int rd);
int mul(int rs, int rt);
int add(int rd, int rs, int rt);
int sub(int rd, int rs, int rt);
int and(int rd, int rs, int rt);
int or(int rd, int rs, int rt);
int xor(int rd, int rs, int rt);
int nor(int rd, int rs, int rt);
int slt(int rd, int rs, int rt);

// J 포맷 명령어
int j(int address);
int jal(int address);

// I 포맷 명령어
int bltz(int rs, int rt, int imm);
int beq(int rs, int rt, int imm);
int bne(int rs, int rt, int imm);
int addi(int rt, int rs, int imm);
int slti(int rt, int rs, int imm);
int andi(int rt, int rs, int imm);
int ori(int rt, int rs, int imm);
int xori(int rt, int rs, int imm);
int lui(int rt, int imm);
int lb(int rt, int imm, int rs);
int lw(int rt, int imm, int rs);
int lbu(int rt, int imm, int rs);
int sb(int rt, int imm, int rs);
int sw(int rt, int imm, int rs);
