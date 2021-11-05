#ifndef _STL_H
#define _STL_H

#include <stdint.h>
#include <avr/io.h>

#define FUNC_BIT_POS 40
#define FUNC_PARAM_MASK 0xFFFFFFFFFFULL
#define MAX_PROGRAM_SIZE 10
#define A 1ULL
#define O 2ULL
#define AN 3ULL
#define ON 4ULL
#define ASGN 5ULL
#define S 6ULL
#define R 7ULL
#define FP 8ULL
#define FN 9ULL
#define L 10ULL
#define T 11ULL

#define Q 1ULL
#define I 2ULL
#define M 3ULL
#define MB 4ULL
#define MW 5ULL
#define MD 6ULL
#define CS 7ULL

#define s_stll_v(a1, a2, a3) ((a1<<40ULL) | ((a2) << (32ULL)) | (a3 & 0xFFFFFFFF))
#define s_stll_m(a1, a2, a3, a4) ((a1<<40ULL) | ((a2) << (32ULL)) | (a3<<4) | (a4))
#define set_b(m, p, b) *memMap[m][p] |= _BV(b);
#define reset_b(m, p, b) *memMap[m][p] &= ~_BV(b);

typedef union {
  uint8_t b[4];
  uint16_t w[2];
  uint32_t dw;
}mem_u;

extern uint64_t program[MAX_PROGRAM_SIZE];
extern uint8_t volatile PC;
extern uint8_t m[64];

void _and(uint64_t param);
void _or(uint64_t param);
void _nand(uint64_t param);
void _nor(uint64_t param);
void _not(uint64_t param);
void _assign(uint64_t param);
void _s(uint64_t param);
void _r(uint64_t param);
void _fp(uint64_t param);
void _fn(uint64_t param);
void _l(uint64_t param);
void _t(uint64_t param);

extern void setupMem();
extern void executeCommandAt(int pl);
extern void executeCommand(uint64_t param);

#endif //_STL_H
