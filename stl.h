#ifndef _STL_H
#define _STL_H

#include <stdint.h>
#include <avr/io.h>

#define FUNC_BIT_POS 40
#define FUNC_PARAM_MASK 0xFFFFFFFFFFULL
#define MAX_PROGRAM_SIZE 15
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

#define CU 18ULL
#define CD 19ULL
#define SC 20ULL
#define RC 21ULL
#define LC 22ULL
#define CLC 23ULL

#define ADDI 24ULL
#define SUBI 25ULL
#define MULI 26ULL
#define DIVI 27ULL
#define ADDD 28ULL
#define SUBD 29ULL
#define MULD 30ULL
#define DIVD 31ULL
#define ADDR 32ULL
#define SUBR 33ULL
#define MULR 34ULL
#define DIVR 35ULL

#define EQI 36ULL
#define DIFFI 37ULL
#define GTI 38ULL
#define LTI 39ULL
#define GTEQI 40ULL
#define LTEQI 41ULL

#define EQD 42ULL
#define DIFFD 43ULL
#define GTD 44ULL
#define LTD 45ULL
#define GTEQD 46ULL
#define LTEQD 47ULL

#define EQR 48ULL
#define DIFFR 49ULL
#define GTR 50ULL
#define LTR 51ULL
#define GTEQR 52ULL
#define LTEQR 53ULL

#define Q 1ULL
#define I 2ULL
#define M 3ULL
#define TIM 4ULL
#define MB 5ULL
#define MW 6ULL
#define MD 7ULL
#define CS 8ULL
#define CN 9ULL

#define s_stll_v(a1, a2, a3) ((a1<<40ULL) | ((a2) << (32ULL)) | (a3 & 0xFFFFFFFF))
#define s_stll_m(a1, a2, a3, a4) ((a1<<40ULL) | ((a2) << (32ULL)) | (a3<<4) | (a4))
#define s_stll_s(a1) (a1<<40ULL)
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

//_cu, _cd, _cs, _cr, _cl, _clc

void _cu(uint64_t param);
void _cd(uint64_t param);
void _cs(uint64_t param);
void _cr(uint64_t param);
void _cl(uint64_t param);
void _clc(uint64_t param);

void _addI(uint64_t param);
void _subI(uint64_t param);
void _mulI(uint64_t param);
void _divI(uint64_t param);

void _addD(uint64_t param);
void _subD(uint64_t param);
void _mulD(uint64_t param);
void _divD(uint64_t param);

void _addR(uint64_t param);
void _subR(uint64_t param);
void _mulR(uint64_t param);
void _divR(uint64_t param);

void _eqI(uint64_t param);
void _diffI(uint64_t param);
void _gtI(uint64_t param);
void _ltI(uint64_t param);
void _gteqI(uint64_t param);
void _lteqI(uint64_t param);

void _eqD(uint64_t param);
void _diffD(uint64_t param);
void _gtD(uint64_t param);
void _ltD(uint64_t param);
void _gteqD(uint64_t param);
void _lteqD(uint64_t param);

void _eqR(uint64_t param);
void _diffR(uint64_t param);
void _gtR(uint64_t param);
void _ltR(uint64_t param);
void _gteqR(uint64_t param);
void _lteqR(uint64_t param);

extern void setupMem();
extern void afterFirstScan();
extern void executeCommandAt(int pl);
extern void executeCommand(uint64_t param);
extern void timersRoutine();

#endif //_STL_H
