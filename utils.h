#include <stdio.h>

// 최대로 파싱할 명령어 + 인자 개수
#define MAX_ARGC 3

// 문자를 파싱해서 숫자로 반환하는 함수
unsigned int parseInt(char* str);

// 파일 포인터에서 1문자 씩 읽어서 인자 c에 저장하는 함수
// 읽는 데 성공하면 0, 실패하면 1 반환
int readChar(FILE *fp, unsigned char *c);

// 8비트 씩 순서를 뒤집어서 빅 엔디안을 리틀 엔디안으로 변환하는 함수
unsigned int toLittleEndian(unsigned int n);

// 입력 받은 명령어를 commands 문자열 배열에 집어 넣음
int readCommands(char** commands);

// 프로그램 불러오기. l 명령어 입력 시 실행됨
// 하는 일: 1. 파일을 읽기, 2. PC, 레지스터, 메모리 초기화 3. 파일 내용을 메모리에 적재 
void loadProgram(const char* filename);

// 디버그 메시지 출력
int printDebug(const char *format, ...);

// 성공 메시지 초록색 출력
int printSuccess(const char *format, ...);

// 에러 메시지 출력
int printError(const char *format, ...);