#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "register.h"
#include "decode.h"
#include "utils.h"
#include "alu.h"
#include "execute.h"
#include "execute.test.h"

// register.c에 있는 PC와 SP 값을 참조함
extern int PC;

int main(void) {
  while(1) {
    char* commands[MAX_ARGC] = { "", "", "" };
    // 명령어는 commands에, 명령어 개수는 argc에 저장함
    // commands[0]은 l, j 같은 실제 명령어, commands[1]과 commands[2]는 메모리 주소 같은 인자를 나타냄
    int argc = readCommands(commands);

    switch (commands[0][0]) {
      case 'l': // l <실행 파일이름>.  사용예시) l 1.bin
        printDebug("[프로그램 읽어오기] 파일명: %s\n", commands[1]);
        char* filename = commands[1];
        loadProgram(filename);
        break;
      case 'j': // j <프로그램 시작 위치>
        printDebug("[프로그램 점프하기] 주소: %s\n", commands[1]);
        unsigned int address = parseInt(commands[1]);
        jumpProgram(address);
        break;
      case 'g': // Go program
        printDebug("[프로그램 전체 실행]\n");
        goProgram();
        break;
      case 's':
        switch (commands[0][1]) {
          case 0: // 명령어 s
            printDebug("[프로그램 명령어 한 단계 실행]\n");
            stepProgram();
            break;
          case 'm': // sm <location> <value>: 메모리 특정 주소의 값 설정
            printDebug("[프로그램 메모리 설정] 주소: %s, 값: %s\n", commands[1], commands[2]);
            unsigned int address = parseInt(commands[1]);
            int memory_value = parseInt(commands[2]);
            setMemory(address, memory_value);
            break;
          case 'r': // sr <register number> <value>: 특정 레지스터의 값 설정
            printDebug("[프로그램 레지스터 설정] 인덱스: %s, 값: %s", commands[1], commands[2]);
            unsigned int number = parseInt(commands[1]);
            int register_value = parseInt(commands[2]);
            setRegister(number, register_value);
            break;
        }
        break;
      case 'm': // m <start> <end>: start ~ end 범위의 메모리 내용 출력
        printDebug("[메모리 출력] 시작: %s, 끝: %s\n", commands[1], commands[2]);
        unsigned int start = parseInt(commands[1]);
        unsigned int end = parseInt(commands[2]);
        viewMemory(start, end);
        break;
      case 'r': // 현재 레지스터 내용 출력
        printDebug("[레지스터 출력]\n");
        viewRegister();
        break;
      case 'x': // 시뮬레이터 프로그램의 종료
        printDebug("[프로그램 종료]\n");
        exit(1);
      case 'c': // 화면 지우기
        printf("\e[1;1H\e[2J");
        break;
      case 't': // test 실행
        printDebug("[테스트 실행]\n");
        executeTest();
        break;
      default:
        printError("[ERROR] 잘못된 명령어입니다.\n");
    }

    for (int i = 0; i < argc; i++) free(commands[i]); // 동적할당한 commands 문자배열 할당 해체
  }
  return 0;
}