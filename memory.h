#define MEMORY_SIZE 0x100000

enum SIZE { BYTE, HALF_WORD, WORD };

enum READ_WRITE { READ, WRITE };

// 메모리 접근 인터페이스
int MEM(unsigned int A, int V, int nRW, int S);

// start 주소부터 end 주소까지의 메모리 출력하기
// m <start> <end> 명령어 입력 시 실행됨
// ex) m 0x400000 0x400010
void viewMemory(unsigned int start, unsigned int end);

// 프로그램, 데이터, 스택 메모리 값 모두 0으로 설정
void resetMemory();

// address의 주소를 value 값으로 설정
// sm 명령어 입력 시 실행됨
void setMemory(unsigned int address, int value);