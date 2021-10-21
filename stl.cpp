#include <Arduino.h>
#include "stl.h"
#include "messages.h"
#include "gui.h"

uint64_t program[MAX_PROGRAM_SIZE];
uint8_t nullByte;

void _nop(uint64_t param);

void (*func_ptr[])(uint64_t) = {_nop, _and, _or, _nand, _nor, _assign, _s, _r};
mem_u m[4];

uint8_t volatile * const mem_p[] = {&nullByte, &PORTB, &m[0].b[0], &m[0].b[1], &m[0].b[2], &m[0].b[3] };
uint8_t volatile RLO = 0;
uint8_t volatile cancel_RLO = true;
uint8_t volatile PC = 0;

void print_binary(int number, uint8_t len){
  static int bits;
  if(number){
    bits++;
    print_binary(number >> 1, len);
    if(bits)for(uint8_t x = (len - bits);x;x--)Serial.write('0');
    bits=0;
    Serial.write((number & 1)?'1':'0');
  }
}

void mem_print(uint64_t param){
  uint8_t func_id = param >> FUNC_BIT_POS;
  uint8_t mem_pos = param >> 32;
  uint8_t bit_pos = param & 0x7;
  int val = ((*mem_p[mem_pos] & (1<<bit_pos))>0);
  char buf[10];
  printAtoBuf(comStr, func_id, buf); 
  Serial.print(buf);
  Serial.print(" ");
  printAtoBuf(memStr, mem_pos, buf); 
  Serial.print(buf);
  Serial.print(".");Serial.print(bit_pos);
  Serial.print("=");Serial.print(val);
  Serial.print(" RLO=");Serial.println(RLO, BIN);
}

void setupMem(){
  set_b(M0, 0);
  set_b(M1, 0);
}

void executeCommand(uint64_t instr){
  uint8_t func_id = instr >> FUNC_BIT_POS;
  //uint64_t param = instr & FUNC_PARAM_MASK;
  (*func_ptr[func_id])(instr);
  mem_print(instr);
}

void executeCommandAt(int pl){
  executeCommand(program[pl]);
}

void _nop(uint64_t param){}

void _and(uint64_t param){
  uint8_t mem_pos = param >> 32;
  uint8_t bit_pos = param & 0x7;
  if(cancel_RLO) RLO = (*mem_p[mem_pos]>>bit_pos) & 0x1;
  else RLO &= (*mem_p[mem_pos]>>bit_pos) & 0x1;
  cancel_RLO = false;
}

void _nand(uint64_t param){
  uint8_t mem_pos = param >> 32;
  uint8_t bit_pos = param & 0x7;
  if(cancel_RLO) RLO = ~(*mem_p[mem_pos]>>bit_pos) & 0x1;
  else RLO &= ~(*mem_p[mem_pos]>>bit_pos) & 0x1;
  cancel_RLO = false;
}

void _or(uint64_t param){
  uint8_t mem_pos = param >> 32;
  uint8_t bit_pos = param & 0x7;
  if(cancel_RLO) RLO = (*mem_p[mem_pos]>>bit_pos) & 0x1;
  else RLO |= (*mem_p[mem_pos]>>bit_pos) & 0x1; 
  cancel_RLO = false;
}

void _nor(uint64_t param){
  uint8_t mem_pos = param >> 32;
  uint8_t bit_pos = param & 0x7;
  if(cancel_RLO) RLO = ~(*mem_p[mem_pos]>>bit_pos) & 0x1;
  else RLO |= ~(*mem_p[mem_pos]>>bit_pos) & 0x1; 
  cancel_RLO = false;
}

void _assign(uint64_t param){
  uint8_t mem_pos = param >> 32;
  uint8_t bit_pos = param & 0x7;
  uint8_t mask = 1 << bit_pos;
  *mem_p[mem_pos] = ((*mem_p[mem_pos] & ~mask) | RLO << bit_pos);
  cancel_RLO = true;
}

void _s(uint64_t param){
  uint8_t mem_pos = param >> 32;
  uint8_t bit_pos = param & 0x7;
  uint8_t mask = 1 << bit_pos;
  if(RLO==0x1)
    *mem_p[mem_pos] = ((*mem_p[mem_pos] & ~mask) | RLO << bit_pos);
  cancel_RLO = true;
}

void _r(uint64_t param){
  uint8_t mem_pos = param >> 32;
  uint8_t bit_pos = param & 0x7;
  uint8_t mask = 1 << bit_pos;
  if(RLO==0x1)
    *mem_p[mem_pos] = ((*mem_p[mem_pos] & ~mask));
  cancel_RLO = true;
}
