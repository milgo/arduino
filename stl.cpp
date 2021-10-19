#include <Arduino.h>
#include "stl.h"

uint64_t program[MAX_PROGRAM_SIZE];

void (*func_ptr[])(uint64_t) = {_and, _or, _assign};

union markers{
  uint8_t b[4];
  uint16_t w[2];
  uint32_t dw;
}m[4];

uint8_t volatile * const mem_p[] = { &PORTB, &m[0].b[0], &m[0].b[1], &m[0].b[2], &m[0].b[3] };
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
  int mem_pos = param >> 32;
  int bit_pos = param & 0x7;
  int val = ((*mem_p[mem_pos] & (1<<bit_pos))>0);
  Serial.print(" M");Serial.print(mem_pos-1);
  //v = param & 0xFFFFFFFF;
  Serial.print(". ");Serial.print(bit_pos);
  //v = ((*mem_p[mem_pos] & (1<<bit_pos))>0)
  Serial.print("= ");Serial.print(val);
  Serial.print(", RLO = ");Serial.println(RLO, BIN);
}

void setupMem(){
  set_b(M0, 0);
  set_b(M1, 0);
  set_b(M2, 4);
}

void executeCommand(int pl){
  uint8_t func_id = program[pl] >> FUNC_BIT_POS;
  uint64_t param = program[pl] & FUNC_PARAM_MASK;
  (*func_ptr[func_id-1])(param);
}

void _and(uint64_t param){
  Serial.print("_and -> ");

  int mem_pos = param >> 32;
  int bit_pos = param & 0x7;

  //RLO |= cancel_RLO;
  if(cancel_RLO) RLO = (*mem_p[mem_pos]>>bit_pos);
  else RLO &= (*mem_p[mem_pos]>>bit_pos);

  //RLO = (*mem_p[mem_pos]>>bit_pos) & ((RLO<<cancel_RLO) & 0x1);
  
  //Serial.print("RLO=");Serial.println(RLO);
  
  cancel_RLO = false;
  mem_print(param);
}


void _or(uint64_t param){
  Serial.print("_or -> ");

  int mem_pos = param >> 32;
  int bit_pos = param & 0x7;

  //RLO |= cancel_RLO;
  if(cancel_RLO) RLO = (*mem_p[mem_pos]>>bit_pos) & 0x1;
  else RLO |= (*mem_p[mem_pos]>>bit_pos) & 0x1; 
  
  //RLO = (*mem_p[mem_pos]>>bit_pos) | ((RLO<<cancel_RLO) & 0x1);
  
  //Serial.print("RLO=");Serial.println(RLO);
  
  cancel_RLO = false;
  mem_print(param);
}

void _assign(uint64_t param){
  Serial.print("_assign -> ");

  int mem_pos = param >> 32;
  int bit_pos = param & 0x7;

  uint8_t mask = 1 << bit_pos;
  //byte clr_bit = *mem_p[mem_pos] & ~mask;

  *mem_p[mem_pos] = ((*mem_p[mem_pos] & ~mask) | RLO << bit_pos);

  cancel_RLO = true;

  mem_print(param);
}
