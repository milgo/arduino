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

#define Q0 1ULL
#define I0 2ULL
#define I1 3ULL
#define M0 4ULL
#define M1 5ULL

#define s_stll(a1, a2, a3) ((a1<<40ULL) | ((a2) << (32ULL)) | (a3))
#define set_b(m, p) *mem_p[m] |= _BV(p);
#define reset_b(m, p) *mem_p[m] &= ~_BV(p);

typedef union {
  uint8_t b[4];
  uint16_t w[2];
  uint32_t dw;
}mem_u;

extern uint64_t program[MAX_PROGRAM_SIZE];
extern uint8_t volatile PC;
extern mem_u m[4];

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

extern void setupMem();
extern void executeCommandAt(int pl);
extern void executeCommand(uint64_t param);

#endif //_STL_H
