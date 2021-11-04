#include <Arduino.h>
#include "stl.h"
#include "messages.h"
#include "gui.h"

/*

Instruction:  00000000 00000000 00000000  00000000  00000000 00000000 00000000 00000000
             |          Function         |Mem ptr |      Reserved         |Mem id |Bit|
             |          Function         |Mem ptr |            Value                  |

*/

uint64_t program[MAX_PROGRAM_SIZE];
uint64_t accumulator[2];
uint8_t nullByte;

void _nop(uint64_t param);

void (*func_ptr[])(uint64_t) = {_nop, _and, _or, _nand, _nor, _assign, _s, _r, _fp, _fn, _l, _t};
uint8_t m[64];

uint8_t volatile * const memNull[] = {&nullByte};
uint8_t volatile * const memQ[] = {&PORTB};
uint8_t volatile * const memI[] = {&PIND};
uint8_t volatile * const memM[] = {&m[0], &m[1], &m[2], &m[3], &m[4], &m[5], &m[6], &m[7],
                                   &m[8], &m[9], &m[10], &m[11], &m[12], &m[13], &m[14], &m[15],
                                   &m[16], &m[17], &m[18], &m[19], &m[20], &m[21], &m[22], &m[23], 
                                   &m[24], &m[25], &m[26], &m[27], &m[28], &m[29], &m[30], &m[31], 
                                   &m[32], &m[33], &m[34], &m[35], &m[36], &m[37], &m[38], &m[39], 
                                   &m[40], &m[41], &m[42], &m[43], &m[44], &m[45], &m[46], &m[47], 
                                   &m[48], &m[49], &m[50], &m[51], &m[52], &m[53], &m[54], &m[55], 
                                   &m[56], &m[57], &m[58], &m[59], &m[60], &m[61], &m[62], &m[63]};
uint8_t volatile * const memC[] = {&nullByte};
uint8_t volatile * const memT[] = {&nullByte};

uint8_t volatile *const *memMap[] = {
  memNull,
  memQ,
  memI,
  memM,
  memM,
  memM,
  memM,
  memNull,
  memC,
  memT,
};


uint8_t volatile RLO = 0;
uint8_t volatile cancel_RLO = true;
uint8_t volatile PC = 0;

uint8_t mem_ptr, bit_pos, mask, mem_id;

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
  uint8_t mem_ptr = param >> 32;
  uint8_t bit_pos = param & 0x7;
  int val = ((**memMap[mem_ptr] & (1<<bit_pos))>0);
  char buf[10];
  printAtoBuf(comStr, func_id, buf); 
  Serial.print(buf);
  Serial.print(" ");
  printAtoBuf(memStr, mem_ptr, buf); 
  Serial.print(buf);
  Serial.print(".");Serial.print(bit_pos);
  Serial.print("=");Serial.print(val);
  Serial.print(" RLO=");Serial.println(RLO, BIN);
}

void setupMem(){
  set_b(M, 0, 0);
  set_b(M, 1, 0);
}

void executeCommand(uint64_t instr){
  uint8_t func_id = instr >> FUNC_BIT_POS;
  //uint64_t param = instr & FUNC_PARAM_MASK;
  (*func_ptr[func_id])(instr);
  mem_print(instr);
  delay(200);
}

void executeCommandAt(int pl){
  executeCommand(program[pl]);
}

void pushToAcc(uint64_t param){
  accumulator[1] = accumulator[0];
  accumulator[0] = param;
}

void _nop(uint64_t param){}

void _and(uint64_t param){
  mem_ptr = param >> 32 & 0xFF;
  bit_pos = param & 0x7;
  if(cancel_RLO) RLO = (**memMap[mem_ptr]>>bit_pos) & 0x1;
  else RLO &= (**memMap[mem_ptr]>>bit_pos) & 0x1;
  cancel_RLO = false;
}

void _nand(uint64_t param){
  mem_ptr = param >> 32 & 0xFF;
  bit_pos = param & 0x7;
  if(cancel_RLO) RLO = ~(**memMap[mem_ptr]>>bit_pos) & 0x1;
  else RLO &= ~(**memMap[mem_ptr]>>bit_pos) & 0x1;
  cancel_RLO = false;
}

void _or(uint64_t param){
  mem_ptr = param >> 32 & 0xFF;
  bit_pos = param & 0x7;
  if(cancel_RLO) RLO = (**memMap[mem_ptr]>>bit_pos) & 0x1;
  else RLO |= (**memMap[mem_ptr]>>bit_pos) & 0x1; 
  cancel_RLO = false;
}

void _nor(uint64_t param){
  mem_ptr = param >> 32 & 0xFF;
  bit_pos = param & 0x7;
  if(cancel_RLO) RLO = ~(**memMap[mem_ptr]>>bit_pos) & 0x1;
  else RLO |= ~(**memMap[mem_ptr]>>bit_pos) & 0x1; 
  cancel_RLO = false;
}

void _assign(uint64_t param){
  mem_ptr = param >> 32 & 0xFF;
  bit_pos = param & 0x7;
  mask = 1 << bit_pos;
  **memMap[mem_ptr] = ((**memMap[mem_ptr] & ~mask) | RLO << bit_pos);
  cancel_RLO = true;
}

void _s(uint64_t param){
  mem_ptr = param >> 32 & 0xFF;
  bit_pos = param & 0x7;
  mask = 1 << bit_pos;
  if(RLO==0x1)
    **memMap[mem_ptr] = ((**memMap[mem_ptr] & ~mask) | RLO << bit_pos);
  cancel_RLO = true;
}

void _r(uint64_t param){
  mem_ptr = param >> 32 & 0xFF;
  bit_pos = param & 0x7;
  mask = 1 << bit_pos;
  if(RLO==0x1)
    **memMap[mem_ptr] = ((**memMap[mem_ptr] & ~mask));
  cancel_RLO = true;
}

void _fp(uint64_t param){
  mem_ptr = param >> 32 & 0xFF;
  bit_pos = param & 0x7;
  mask = 1 << bit_pos;
  uint8_t m = (**memMap[mem_ptr]>>bit_pos) & 0x1;
  if(RLO == 0)**memMap[mem_ptr] = ((**memMap[mem_ptr] & ~mask));
  if(RLO == 0x1 && m == 0x0){
    RLO = 0x1;
    **memMap[mem_ptr] = ((**memMap[mem_ptr] & ~mask) | 1 << bit_pos);
  }else{RLO = 0x0;}
  cancel_RLO = false;
}

void _fn(uint64_t param){
  mem_ptr = (param >> 32) & 0xFF;
  bit_pos = param & 0x7;
  mask = 1 << bit_pos;
  uint8_t m = (**memMap[mem_ptr]>>bit_pos) & 0x1;
  if(RLO == 1)**memMap[mem_ptr] = ((**memMap[mem_ptr] & ~mask) | 1 << bit_pos);
  if(RLO == 0x0 && m == 0x1){
    RLO = 0x1;
    **memMap[mem_ptr] = ((**memMap[mem_ptr] & ~mask));
  }else{RLO = 0x0;}  
  cancel_RLO = false;
}

void _l(uint64_t param){
  mem_ptr = (param >> 32) & 0xFF; 
  mem_id = (param >> 4) & 0xFF; //4,5,6,7

  /*if(mem_ptr == 7){ //const
    pushToAcc(param & 0xFFFFFFFF);
  }else{*/
    mem_id-=4;//0,1,2
    uint8_t bytes = 1 << mem_id;
    for(uint8_t i=0; i< (1<<mem_id); i++)
      
  //}
  
  Serial.print(mem_ptr);Serial.print(" ");Serial.print(mem_id);
  //uint8_t 
  
  //pushToAcc(m[mem_ptr].w[0]);
}

void _t(uint64_t param){
  mem_ptr = param >> 32 & 0xFF;
  //*mem_p[mem_ptr] = accumulator[0];
  **memMap[1] = 2;//accumulator[0];//TUTAJ prawdopodobny nowy sposob odwolywania sie do pamięci
}
