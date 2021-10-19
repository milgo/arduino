#ifndef _STL_H
#define _STL_H

#include <stdint.h>
#include <avr/io.h>

#define FUNC_BIT_POS 40
#define FUNC_PARAM_MASK 0xFFFFFFFFFFULL
#define MAX_PROGRAM_SIZE 10
#define A 1ULL
#define O 2ULL
#define ASGN 3ULL

#define Q0 1ULL
#define M0 2ULL
#define M1 3ULL

#define s_stll(a1, a2, a3) ((a1<<40ULL) | ((a2) << (32ULL)) | (a3))

extern uint64_t program[MAX_PROGRAM_SIZE];
extern uint8_t volatile PC;

void _and(uint64_t param);
void _or(uint64_t param);
void _assign(uint64_t param);

#endif //_STL_H
