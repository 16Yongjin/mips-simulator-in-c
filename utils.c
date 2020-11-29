#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "utils.h"
#include "memory.h"
#include "register.h"
#include "decode.h"

#define BINARY_BUFFER_SIZE 1024

// register.c에 있는 PC와 SP 값을 참조함
extern int PC;

// 문자를 파싱해서 숫자로 반환하는 함수
unsigned int parseInt(char* str) {
  return (unsigned int)strtoul(str, NULL, 0);
}

// 파일 포인터에서 1문자 씩 읽어서 인자 c에 저장하는 함수
// 읽는 데 성공하면 0, 실패하면 1 반환
int readChar(FILE *fp, unsigned char *c) {
  if (fread(c, 1, 1, fp) != 1) return 1;
  else return 0;
}

// 8비트 씩 순서를 뒤집어서 빅 엔디안을 리틀 엔디안으로 변환하는 함수
unsigned int toLittleEndian(unsigned int n){
  return ((n >> 24) & 0xff) | ((n << 8) & 0xff0000) | ((n >> 8) & 0xff00) | ((n << 24) & 0xff000000);
}

// 입력 받은 명령어를 공백으로 나눠서 commands 문자열 배열에 집어 넣음
// 유저입력이 "m 0x123 0x456"이라면 => commands는 ["m", "0x123", "0x456"]
int readCommands(char** commands) {
  // fgets로 명령어 100 글자 읽어온 다음에 commandBuffer에 저장
  printf("명령어를 입력해주세요: ");
  char commandBuffer[100];
  fgets(commandBuffer, 100, stdin);

  // 마지막 개행문자 없애기. 참고: https://bit.ly/3kLgE4L
  // 마지막 개행문자가 없어야 파일 읽기 시 파일이름에 이상이 없음
  strtok(commandBuffer, "\n");

  // 스페이스(" ")로 나눠서 각각의 단어를 commands 배열에 넣음
  // strtok 사용법 참고: https://bit.ly/3oJvATi
  char* token = strtok(commandBuffer, " "); // commandBuffer에서 공백(" ") 전까지 단어 가져오기
  int argc = 0; // 명령어 개수
  while (token != NULL && argc < MAX_ARGC) { // 읽어 들인 단어가 있고 명령어 최대 개수까지 실행
      // printDebug("[DEBUG] 읽어들인 단어 %d: %s\n", argc, token);
      commands[argc] = (char*)malloc((strlen(token)+1) * sizeof(char)); // 명령어를 저장할 공간 확보
      strcpy(commands[argc], token); // 명령어 strcpy로 저장
      argc++; // 명령어 개수 증가
      token = strtok(NULL, " "); // 다음 명령어 읽기
  }

  // 명령어 개수를 의미하는 argc 값 출력해보기
  // printDebug("[DEBUG] 읽은 단어 개수: %d\n", argc);

  // 각각의 명령어 출력해보기
  // for (int i = 0; i < argc; i++) {
  //   printDebug("[DEBUG] commands %d번째 요소:  %s\n", i, commands[i]);
  // }

  return argc;
}


// 프로그램 불러오기. l 명령어 입력 시 실행됨
// 하는 일: 1. 파일을 읽기, 2. PC, 레지스터, 메모리 초기화 3. 파일 내용을 메모리에 적재 
void loadProgram(const char* filename) {
  FILE *fptr = NULL;

  if ((fptr = fopen(filename, "rb")) == NULL) {
    printError("[ERROR] 파일 읽기 오류! \n");
    return;
  }

  // 파일에서 읽은 바이너리를 저장할 버퍼
  unsigned char BINARY_BUFFER[BINARY_BUFFER_SIZE];

  // 바이너리 내용 BINARY_BUFFER에 저장하기
  for (int i = 0; i < BINARY_BUFFER_SIZE; i++) {
    int err = readChar(fptr, &BINARY_BUFFER[i]);
    if (err) break;
  }

  fclose(fptr); // 파일 읽기 끝

  // 명령어, 데이터 길이 구하기
  unsigned int num_instruction = toLittleEndian(((unsigned int*)BINARY_BUFFER)[0]);
  unsigned int num_data = toLittleEndian(((unsigned int*)BINARY_BUFFER)[1]);

  printDebug("[DEBUG] 명령어 개수: %d\n", num_instruction);
  printDebug("[DEBUG] 데이터 개수: %d\n", num_data);

  // 프로그램, 스택 포인터 초기화
  PC = 0x400000;

  // 메모리, 레지스터 초기화
  resetMemory();
  resetRegister();

  // 명령어를 메모리에 적재하기
  for (int i = 0; i < num_instruction; i++) {
    int offset = i + 2; // 명령어 길이, 데이터 길이 워드 다음 인덱스
    unsigned int word = toLittleEndian(((unsigned int*)BINARY_BUFFER)[offset]);
    printDebug("[DEBUG] 읽은 명령어: "); printInstruction(word);
    MEM(0x400000 + 4 * i, word, WRITE, WORD);
  }

  // 데이터를 메모리에 적재하기
  for (int i = 0; i < num_data; i++) {
    int offset = i + 2 + num_instruction; // 명령어 길이, 데이터 길이, 명령어 워드 다음 인덱스
    unsigned int word = toLittleEndian(((unsigned int*)BINARY_BUFFER)[offset]);
    printDebug("[DEBUG] 읽은 데이터: %x\n", word);
    MEM(0x10000000 + 4 * i, word, WRITE, WORD);
  }
}


// 디버깅 메시지 청록색 출력
// 가변인자 사용법 참조: https://bit.ly/2U7oxWy
int printDebug(const char *format, ...) {
   va_list arg;
   int done;

   va_start(arg, format);
   printf("\033[0;36m");
   done = vfprintf(stdout, format, arg);
   printf("\033[0m");
   va_end(arg);

   return done;
}

// 성공 메시지 초록색 출력
int printSuccess(const char *format, ...) {
   va_list arg;
   int done;

   va_start(arg, format);
   printf("\033[0;32m");
   done = vfprintf(stdout, format, arg);
   printf("\033[0m");
   va_end(arg);

   return done;
}

// 에러 메시지 빨간색으로 출력
int printError(const char *format, ...) {
   va_list arg;
   int done;

   va_start(arg, format);
   printf("\033[0;31m");
   done = vfprintf(stdout, format, arg);
   printf("\033[0m");
   va_end(arg);

   return done;
}
