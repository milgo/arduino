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
#define TSP 12ULL
#define TSE 13ULL
#define TSD 14ULL
#define TSS 15ULL
#define TSF 16ULL
#define TRT 17ULL

#define Q 1ULL
#define I 2ULL
#define M 3ULL
#define TIM 4ULL
#define MB 5ULL
#define MW 6ULL
#define MD 7ULL
#define CS 8ULL

#define s_stll_v(a1, a2, a3) ((a1<<40ULL) | ((a2) << (32ULL)) | (a3 & 0xFFFFFFFF))
#define s_stll_m(a1, a2, a3, a4) ((a1<<40ULL) | ((a2) << (32ULL)) | (a3<<4) | (a4))
#define set_b(m, p, b) *memMap[m][p] |= _BV(b);
#define reset_b(m, p, b) *memMap[m][p] &= ~_BV(b);

extern uint64_t program[MAX_PROGRAM_SIZE];
extern uint8_t volatile PC;
extern uint8_t m[64];
extern uint8_t buttons;

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

//_sp, _se, _sd, _ss, _sf, _rt

void _sp(uint64_t param);
void _se(uint64_t param);
void _sd(uint64_t param);
void _ss(uint64_t param);
void _sf(uint64_t param);
void _rt(uint64_t param);

extern void setupMem();
extern void executeCommandAt(int pl);
extern void executeCommand(uint64_t param);
extern void timersRoutine();

#endif //_STL_H
