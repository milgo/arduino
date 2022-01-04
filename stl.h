#ifndef _STL_H
#define _STL_H

#include <stdint.h>
#include <avr/io.h>

#define FUNC_BIT_POS 24
#define MEM_BIT_POS 16
#define FUNC_PARAM_MASK 0xFFFFUL
#define MAX_PROGRAM_SIZE 64

#define DO 1UL
#define DI 2UL
#define M 3UL
#define TIM 4UL
#define MB 5UL
#define MW 6UL
#define MD 7UL
#define AI 8UL
#define AO 9UL
#define CS 10UL
#define CN 11UL
#define AD 12UL

#define s_stll_v(a1, a2, a3) ((a1<<FUNC_BIT_POS) | ((a2) << (MEM_BIT_POS)) | (a3 & FUNC_PARAM_MASK))
#define s_stll_m(a1, a2, a3, a4) ((a1<<FUNC_BIT_POS) | ((a2) << (MEM_BIT_POS)) | (a3<<4) | (a4))
#define s_stll_s(a1) (a1<<FUNC_BIT_POS)
#define set_b(m, p, b) *memMap[m][p] |= _BV(b);
#define reset_b(m, p, b) *memMap[m][p] &= ~_BV(b);

extern uint32_t program[MAX_PROGRAM_SIZE];
extern uint8_t volatile PC;
extern uint8_t volatile PS;
extern uint8_t volatile m[64];
extern uint8_t volatile buttons;

void _and(uint32_t param);
void _or(uint32_t param);
void _nand(uint32_t param);
void _nor(uint32_t param);
void _not(uint32_t param);
void _assign(uint32_t param);
void _s(uint32_t param);
void _r(uint32_t param);
void _fp(uint32_t param);
void _fn(uint32_t param);
void _l(uint32_t param);
void _t(uint32_t param);
//_sp, _se, _sd, _ss, _sf, _rt

void _sp(uint32_t param);
void _se(uint32_t param);
void _sd(uint32_t param);
void _ss(uint32_t param);
void _sf(uint32_t param);
void _rt(uint32_t param);

//_cu, _cd, _cs, _cr, _cl, _clc

void _cu(uint32_t param);
void _cd(uint32_t param);
void _cs(uint32_t param);
void _cr(uint32_t param);
void _cl(uint32_t param);
void _clc(uint32_t param);

void _addI(uint32_t param);
void _subI(uint32_t param);
void _mulI(uint32_t param);
void _divI(uint32_t param);

/*void _addD(uint32_t param);
void _subD(uint32_t param);
void _mulD(uint32_t param);
void _divD(uint32_t param);

void _addR(uint32_t param);
void _subR(uint32_t param);
void _mulR(uint32_t param);
void _divR(uint32_t param);*/

void _eqI(uint32_t param);
void _diffI(uint32_t param);
void _gtI(uint32_t param);
void _ltI(uint32_t param);
void _gteqI(uint32_t param);
void _lteqI(uint32_t param);

/*void _eqD(uint32_t param);
void _diffD(uint32_t param);
void _gtD(uint32_t param);
void _ltD(uint32_t param);
void _gteqD(uint32_t param);
void _lteqD(uint32_t param);

void _eqR(uint32_t param);
void _diffR(uint32_t param);
void _gtR(uint32_t param);
void _ltR(uint32_t param);
void _gteqR(uint32_t param);
void _lteqR(uint32_t param);*/

void _ju(uint32_t param);
void _jc(uint32_t param);
void _jcn(uint32_t param);

void _sda(uint32_t param);
void _smo(uint32_t param);
void _sye(uint32_t param);
void _sho(uint32_t param);
void _smi(uint32_t param);
void _sse(uint32_t param);

void _gda(uint32_t param);
void _gmo(uint32_t param);
void _gye(uint32_t param);
void _gho(uint32_t param);
void _gmi(uint32_t param);
void _gse(uint32_t param);

extern void setupMem();
extern void afterFirstScan();
extern void executeCommandAt(int pl);
extern void executeCommand(uint32_t param);
extern void timersRoutine();
extern void readAnalog();
extern void writeAnalog();

#endif //_STL_H
