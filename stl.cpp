#include <FRAM.h>
#include <MCP7940.h>
#include <Arduino.h>
#include "stl.h"
#include "messages.h"
#include "gui.h"


/*
Instruction:  00000000 00000000 00000000 00000000
             |Function |Mem ptr |Res |Mem id |Bit|
             |Function |Mem ptr | Value          |
*/

uint32_t program[MAX_PROGRAM_SIZE];
int32_t accumulator[2];
uint8_t nullByte;

void _nop(uint32_t param);

void (*func_ptr[])(uint32_t) = {_nop, _and, _or, _nand, _nor, _assign, _s, _r, _fp, _fn, _pb, _l, _t, _pv, /**/_sp, _se, _sd, _ss, _sf, _rt, _cu, _cd, _cs, _cr, _cl, _clc,
 _addI, _subI, _mulI, _divI, /*_addD, _subD, _mulD, _divD, _addR, _subR, _mulR, _divR,*/
 _eqI, _diffI, _gtI, _ltI, _gteqI, _lteqI, /*_eqD, _diffD, _gtD, _ltD, _gteqD, _lteqD, _eqR, _diffR, _gtR, _ltR, _gteqR, _lteqR,*/
 _ju, _jc, _jcn,
 _gda, _gmo, _gye, _gho, _gmi, _gse, _sda, _smo, _sye, _sho, _smi, _sse};
 
uint8_t volatile buttons;
uint8_t volatile m[64];
uint8_t volatile t[8];
uint8_t volatile c;

uint8_t volatile * const memNull[] = {&nullByte};
uint8_t volatile * const memQ[] = {&PORTD};
uint8_t volatile * const memI[] = {&buttons, &PINC, &PINB};
uint8_t volatile * const memM[] = {&m[0], &m[1], &m[2], &m[3], &m[4], &m[5], &m[6], &m[7],
                                   &m[8], &m[9], &m[10], &m[11], &m[12], &m[13], &m[14], &m[15],
                                   &m[16], &m[17], &m[18], &m[19], &m[20], &m[21], &m[22], &m[23], 
                                   &m[24], &m[25], &m[26], &m[27], &m[28], &m[29], &m[30], &m[31], 
                                   &m[32], &m[33], &m[34], &m[35], &m[36], &m[37], &m[38], &m[39], 
                                   &m[40], &m[41], &m[42], &m[43], &m[44], &m[45], &m[46], &m[47], 
                                   &m[48], &m[49], &m[50], &m[51], &m[52], &m[53], &m[54], &m[55], 
                                   &m[56], &m[57], &m[58], &m[59], &m[60], &m[61], &m[62], &m[63]};
uint8_t volatile * const memC[] = {&c};
uint8_t volatile * const memT[] = {&t[0], &t[1], &t[2], &t[3], &t[4], &t[5], &t[6], &t[7]};

uint8_t volatile fixedTimer[8];
uint32_t volatile timer[8];
int32_t volatile counter[8];

MCP7940_Class MCP7940;
FRAM Fram(0b00);

const PROGMEM uint8_t fixedTimerTime[]  = {10, 20, 40, 50, 80, 100, 160, 200};

uint8_t volatile *const *memMap[] = {
  memNull,
  memQ,
  memI,
  memM,
  memT,
  memM,
  memM,
  memM,
  memNull,
  memC,
  memNull
};

uint8_t volatile RLO = 1;
uint8_t volatile cancel_RLO = true;
uint8_t volatile PC = 0, PS = 0;

uint8_t mem_ptr, bit_pos, mask, mem_id, addr;

void print_binary(int number, uint8_t len){
  /*static int bits;
  if(number){
    bits++;
    print_binary(number >> 1, len);
    if(bits)for(uint8_t x = (len - bits);x;x--)Serial.write('0');
    bits=0;
    Serial.write((number & 1)?'1':'0');
  }*/
}

void mem_print(uint32_t param){
  /*uint8_t func_id = param >> FUNC_BIT_POS;
  mem_ptr = param >> 32;
  mem_id = (param >> 4) & 0xFF;
  bit_pos = param & 0x7;
  int val = ((*memMap[mem_ptr][mem_id] & (1<<bit_pos))>0);
  char buf[10];
  printAtoBuf(comStr, func_id, buf); 
  Serial.print(buf);
  Serial.print(" ");
  printAtoBuf(memStr, mem_ptr, buf); 
  Serial.print(buf);
  Serial.print(mem_id);
  Serial.print(".");Serial.print(bit_pos);
  Serial.print("=");Serial.print(val);
  Serial.print(" RLO=");Serial.print(RLO, BIN);
  double d = accumulator[0];
  Serial.print(" ACC=");Serial.print((long int)(accumulator[0]>>32));Serial.print((long int)accumulator[0]);
  Serial.print(" ACCD=");Serial.println(d);*/
}

void extractParams(uint32_t param){
  mem_ptr = (param >> MEM_BIT_POS) & 0xFF;
  mem_id = param >> 4 & 0xFF;
  bit_pos = param & 0x7;
}

void loadFromFram(){
  //Serial.println(F("loading from FRAM"));
  for(int i=0; i<=PS; i++){
    uint8_t func_id = program[i] >> FUNC_BIT_POS;

    if(func_id == PV || func_id == PB){

      extractParams(program[i]);
      uint32_t framVal = 0;
      uint8_t type = mem_ptr-5;//0,1,2,3,4
      uint8_t bytes = 1 << type; //byte, word, dword

      //load from fram
      for(uint8_t i=0; i<bytes; i++){
          uint32_t t = Fram.ReadByte(0, mem_id*bytes+i);
          
          framVal += t<<(i*8); 
      }

      if(func_id == PV){
        //Serial.print(F("PV:"));Serial.println(framVal);
        for(uint8_t i=0; i<bytes; i++){
          *memMap[mem_ptr][mem_id*bytes+i] = framVal>>(i*8)&0xFF;
        }
      }else if(func_id == PB){
        //Serial.print(F("PB:"));Serial.println(framVal);
        mask = 1 << bit_pos;
        *memMap[mem_ptr][mem_id*bytes+i] = (*memMap[mem_ptr][mem_id*bytes+i] & ~mask) | (framVal & mask);
      }
      
    }
  }
}

void setupMem(){
  //set_b(M, 0, 0);
  //set_b(M, 1, 0);
  m[1] |=  1 << 0; //FirstScan
  m[1] |=  0 << 1; //DiagStatusUpdate
  m[1] |=  1 << 2; //AlwaysTrue
  m[1] |=  0 << 3; //AlwaysFalse

  m[1] |=  0 << 4; //Display 1 value dw29
  m[1] |=  0 << 5; //Display 2 value dw30
  m[1] |=  0 << 6; //Display 3 value dw31

  while (!MCP7940.begin()) {                                                  // Initialize RTC communications    //
    Serial.println(F("Unable to find MCP7940M. Checking again in 3s."));      // Show error text                  //
    delay(3000);                                                              // wait a second                    //
  }

  Serial.println(F("MCP7940 initialized."));                                  //                                  //
  while (!MCP7940.deviceStatus()) {                                           // Turn oscillator on if necessary  //
    Serial.println(F("Oscillator is off, turning it on."));                   //                                  //
    bool deviceStatus = MCP7940.deviceStart();                                // Start oscillator and return state//
    if (!deviceStatus) {                                                      // If it didn't start               //
      Serial.println(F("Oscillator did not start, trying again."));           // Show error and                   //
      delay(1000);                                                            // wait for a second                //
    } // of if-then oscillator didn't start                                   //                                  //
  } // of while the oscillator is off                                         //                                  //
  if(!MCP7940.getBattery()){
    MCP7940.setBattery(true);
    Serial.println(F("Battery backup not set. Setting battery backup."));
  }
  else{
    Serial.println(F("Battery backup is set."));
  }

  loadFromFram();
}

void afterFirstScan(){
  m[1] &= ~(1<<0);
}

void timersRoutine(){//10ms
  for(uint8_t i=0; i<8; i++){
    timer[i]+=10;
    
    fixedTimer[i]+=1;
    if(fixedTimer[i] >= pgm_read_byte_near(fixedTimerTime + i)){
      //Serial.println(fixedTimer[i]);
      fixedTimer[i] = 0;
      m[0] ^= 1<<i;
      
    }
  }
}

void executeCommand(uint32_t instr){
  uint8_t func_id = instr >> FUNC_BIT_POS;
  //uint32_t param = instr & FUNC_PARAM_MASK;
  (*func_ptr[func_id])(instr);
  mem_print(instr);
  //delay(200);
}

void executeCommandAt(int pl){
  executeCommand(program[pl]);
}

void pushToAcc(uint32_t param){
  accumulator[1] = accumulator[0];
  accumulator[0] = param;
}

void _nop(uint32_t param){}

void _and(uint32_t param){
  extractParams(param);
  if(cancel_RLO) RLO = (*memMap[mem_ptr][mem_id]>>bit_pos) & 0x1;
  else RLO &= (*memMap[mem_ptr][mem_id]>>bit_pos) & 0x1;
  cancel_RLO = false;
}

void _nand(uint32_t param){
  extractParams(param);
  if(cancel_RLO) RLO = ~(*memMap[mem_ptr][mem_id]>>bit_pos) & 0x1;
  else RLO &= ~(*memMap[mem_ptr][mem_id]>>bit_pos) & 0x1;
  cancel_RLO = false;
}

void _or(uint32_t param){
  extractParams(param);
  if(cancel_RLO) RLO = (*memMap[mem_ptr][mem_id]>>bit_pos) & 0x1;
  else RLO |= (*memMap[mem_ptr][mem_id]>>bit_pos) & 0x1; 
  cancel_RLO = false;
}

void _nor(uint32_t param){
  extractParams(param);
  if(cancel_RLO) RLO = ~(*memMap[mem_ptr][mem_id]>>bit_pos) & 0x1;
  else RLO |= ~(*memMap[mem_ptr][mem_id]>>bit_pos) & 0x1; 
  cancel_RLO = false;
}

void _assign(uint32_t param){
  extractParams(param);
  mask = 1 << bit_pos;
  *memMap[mem_ptr][mem_id] = ((*memMap[mem_ptr][mem_id] & ~mask) | RLO << bit_pos);
  cancel_RLO = true;
}

void _s(uint32_t param){
  extractParams(param);
  mask = 1 << bit_pos;
  if(RLO==0x1)
    *memMap[mem_ptr][mem_id] = ((*memMap[mem_ptr][mem_id] & ~mask) | RLO << bit_pos);
  cancel_RLO = true;
}

void _r(uint32_t param){
  extractParams(param);
  mask = 1 << bit_pos;
  if(RLO==0x1)
    *memMap[mem_ptr][mem_id] = ((*memMap[mem_ptr][mem_id] & ~mask));
  cancel_RLO = true;
}

void _fp(uint32_t param){
  extractParams(param);
  mask = 1 << bit_pos;
  uint8_t m = (*memMap[mem_ptr][mem_id]>>bit_pos) & 0x1;
  if(RLO == 0)*memMap[mem_ptr][mem_id] = ((*memMap[mem_ptr][mem_id] & ~mask));
  if(RLO == 0x1 && m == 0x0){
    RLO = 0x1;
    *memMap[mem_ptr][mem_id] = ((*memMap[mem_ptr][mem_id] & ~mask) | 1 << bit_pos);
  }else{RLO = 0x0;}
  cancel_RLO = false;
}

void _fn(uint32_t param){
  extractParams(param);
  mask = 1 << bit_pos;
  uint8_t m = (*memMap[mem_ptr][mem_id]>>bit_pos) & 0x1;
  if(RLO == 1)*memMap[mem_ptr][mem_id] = ((*memMap[mem_ptr][mem_id] & ~mask) | 1 << bit_pos);
  if(RLO == 0x0 && m == 0x1){
    RLO = 0x1;
    *memMap[mem_ptr][mem_id] = ((*memMap[mem_ptr][mem_id] & ~mask));
  }else{RLO = 0x0;}  
  cancel_RLO = false;
}

void _pb(uint32_t param){
  extractParams(param);
  mask = 1 << bit_pos;
  uint8_t r = Fram.ReadByte(0, mem_id);
  if(((r & mask) ^ (*memMap[mem_ptr][mem_id] & mask)) > 0){
    r = (r & ~mask) | (*memMap[mem_ptr][mem_id] & mask);
    Fram.WriteByte(0, mem_id, r);
  }
}

void _l(uint32_t param){
  extractParams(param);
  
  uint32_t temp = 0;
  uint8_t type = mem_ptr-5;//0,1,2,3,4
  uint8_t bytes = 1 << type; //byte, word, dword

 if(mem_ptr == CS){ //const
    temp = param & 0xFFFF;
  }
  else{
    for(uint8_t i=0; i<bytes; i++){
      uint32_t t = *memMap[mem_ptr][mem_id*bytes+i];
      temp += t<<(i*8); 
   }
  }
  pushToAcc(temp);
  
}

void _t(uint32_t param){
  extractParams(param);
  
  uint32_t temp = 0;
  uint8_t type = mem_ptr-5;//0,1,2,3,4
  uint8_t bytes = 1 << type; //byte, word, dword

  for(uint8_t i=0; i<bytes; i++){
    *memMap[mem_ptr][mem_id*bytes+i] = accumulator[0]>>(i*8)&0xFF; 
  }

  accumulator[0] = 0;
}

void _pv(uint32_t param){
  extractParams(param);
  
  uint32_t framVal = 0, ramVal = 0;
  uint8_t type = mem_ptr-5;//0,1,2,3,4
  uint8_t bytes = 1 << type; //byte, word, dword

  //load from fram
  for(uint8_t i=0; i<bytes; i++){
      uint32_t t = Fram.ReadByte(0, mem_id*bytes+i);
      framVal += t<<(i*8); 
  }
  //Serial.print("fram: ");Serial.println(framVal);

  //load from ram
  for(uint8_t i=0; i<bytes; i++){
      uint32_t t = *memMap[mem_ptr][mem_id*bytes+i];
      ramVal += t<<(i*8); 
  }

  //Serial.print("ram: ");Serial.println(ramVal);

  if(ramVal != framVal){
    //Serial.println("diff!");
    for(uint8_t i=0; i<bytes; i++){
      //*memMap[mem_ptr][mem_id*bytes+i] = accumulator[0]>>(i*8)&0xFF;
      Fram.WriteByte(0, mem_id*bytes+i, ramVal>>(i*8)&0xFF);
    }
  }
  
}

void _sp(uint32_t param){
  extractParams(param);

  if(RLO == 1){

    //start
    if((*memMap[mem_ptr][mem_id] & 0x2) == 0){
      *memMap[mem_ptr][mem_id] = 0x3;
      timer[mem_id]=0;
    }

    //stop
    if(timer[mem_id]>accumulator[0] && (*memMap[mem_ptr][mem_id] & 0x2)){
      *memMap[mem_ptr][mem_id] &= ~(0x1);
    }
    
  }else{
    *memMap[mem_ptr][mem_id] &= ~(0x3);
  }
  cancel_RLO = true;
}

void _se(uint32_t param){
  extractParams(param);

  if(RLO == 1){

    //start
    if((*memMap[mem_ptr][mem_id] & 0x2) == 0){
      *memMap[mem_ptr][mem_id] = 0x3;
      timer[mem_id]=0;
    }
  }

  //stop
  if(timer[mem_id]>accumulator[0] && (*memMap[mem_ptr][mem_id] & 0x2)){
    *memMap[mem_ptr][mem_id] &= ~(0x3);
  }

  cancel_RLO = true;
}

void _sd(uint32_t param){
  extractParams(param);

  if(RLO == 1){

    //start
    if((*memMap[mem_ptr][mem_id] & 0x2) == 0){
      *memMap[mem_ptr][mem_id] = 0x2;
      timer[mem_id]=0;
    }

    //stop
    if(timer[mem_id]>accumulator[0] && (*memMap[mem_ptr][mem_id] & 0x2)){
      *memMap[mem_ptr][mem_id] |= 0x1;
    }
    
  }else{
    *memMap[mem_ptr][mem_id] &= ~(0x3);
  }

  cancel_RLO = true;
}

void _ss(uint32_t param){
  extractParams(param);

  if(RLO == 1){

    //start
    if((*memMap[mem_ptr][mem_id] & 0x2) == 0){
      *memMap[mem_ptr][mem_id] = 0x2;
      timer[mem_id]=0;
    }
    
  }else{
    //*memMap[mem_ptr][mem_id] &= ~(0x3);
  }

  //stop
  if(timer[mem_id]>accumulator[0] && (*memMap[mem_ptr][mem_id] & 0x2)){
    *memMap[mem_ptr][mem_id] |= 0x1;
  }

  cancel_RLO = true;
}

void _sf(uint32_t param){
  extractParams(param);

  if(RLO == 1){

    //start
    if((*memMap[mem_ptr][mem_id] & 0x2) == 0){
      *memMap[mem_ptr][mem_id] |= 0x1;
    }
    
  }else{
    if((*memMap[mem_ptr][mem_id] & 0x3) == 0x1){
      *memMap[mem_ptr][mem_id] |= 0x3;
      timer[mem_id]=0;
    }
    
    if(timer[mem_id]>accumulator[0] && (*memMap[mem_ptr][mem_id] & 0x2)){
      *memMap[mem_ptr][mem_id] &= ~(0x3);
    }
  }

  cancel_RLO = true;  
}

void _rt(uint32_t param){
  extractParams(param);

  if(RLO == 1){
    *memMap[mem_ptr][mem_id] &= ~(0x3);
  }
  cancel_RLO = true;
}

void _cu(uint32_t param){
  mem_id = (param >> 4) & 0xFF;
  if(RLO == 1){
    counter[mem_id]++;
  }
  cancel_RLO = true;
}

void _cd(uint32_t param){
  mem_id = (param >> 4) & 0xFF;
  if(RLO == 1){
    counter[mem_id]--;
  }
  cancel_RLO = true;
}

void _cs(uint32_t param){
  mem_id = (param >> 4) & 0xFF;
  if(RLO == 1){
    counter[mem_id]=accumulator[0];
  }
  cancel_RLO = true;
}

void _cr(uint32_t param){
  mem_id = (param >> 4) & 0xFF;
  if(RLO == 1){
    counter[mem_id]=0;
  }
  cancel_RLO = true;
}

void _cl(uint32_t param){
  mem_id = (param >> 4) & 0xFF;
  if(RLO == 1){
    accumulator[0] = counter[mem_id];
  }
  cancel_RLO = true;
}

void _clc(uint32_t param){
  
}

int32_t accI0 = 0, accI1 = 0;
void _addI(uint32_t param){accI0 = (int32_t)(accumulator[1])+(int32_t)(accumulator[0]); accumulator[0] = accI0;}
void _subI(uint32_t param){accI0 = (int32_t)(accumulator[1])-(int32_t)(accumulator[0]); accumulator[0] = accI0;}
void _mulI(uint32_t param){accI0 = (int32_t)(accumulator[1])*(int32_t)(accumulator[0]); accumulator[0] = accI0;}
void _divI(uint32_t param){accI0 = (int32_t)(accumulator[1])/(int32_t)(accumulator[0]); accumulator[0] = accI0;}

/*int64_t accD0 = 0, accD1 = 0;
void _addD(uint32_t param){accD0 = (int64_t)(accumulator[1])+(int64_t)(accumulator[0]); accumulator[0] = accD0;}
void _subD(uint32_t param){accD0 = (int64_t)(accumulator[1])-(int64_t)(accumulator[0]); accumulator[0] = accD0;}
void _mulD(uint32_t param){accD0 = (int64_t)(accumulator[1])*(int64_t)(accumulator[0]); accumulator[0] = accD0;}
void _divD(uint32_t param){accD0 = (int64_t)(accumulator[1])/(int64_t)(accumulator[0]); accumulator[0] = accD0;}

double accR0 = 0.0, accR1 = 0.0;
void _addR(uint32_t param){accR0 = (double)(accumulator[1])+(double)(accumulator[0]); accumulator[0] = accR0;}
void _subR(uint32_t param){accR0 = (double)(accumulator[1])-(double)(accumulator[0]); accumulator[0] = accR0;}
void _mulR(uint32_t param){accR0 = (double)(accumulator[1])*(double)(accumulator[0]); accumulator[0] = accR0;}
void _divR(uint32_t param){accR0 = (double)(accumulator[1])/(double)(accumulator[0]); accumulator[0] = accR0;}*/

void loadIFromAcc(){
  accI0 = (uint32_t)accumulator[0]; accI1 = (uint32_t)accumulator[1];
}
 
void _eqI(uint32_t param){loadIFromAcc(); RLO=(accI1==accI0); cancel_RLO=false;}
void _diffI(uint32_t param){loadIFromAcc(); RLO=(accI1!=accI0); cancel_RLO=false;}
void _gtI(uint32_t param){loadIFromAcc(); RLO=(accI1>accI0); cancel_RLO=false;}
void _ltI(uint32_t param){loadIFromAcc(); RLO=(accI1<accI0); cancel_RLO=false;}
void _gteqI(uint32_t param){loadIFromAcc(); RLO=(accI1>=accI0); cancel_RLO=false;}
void _lteqI(uint32_t param){loadIFromAcc(); RLO=(accI1<=accI0); cancel_RLO=false;}

/*void loadDFromAcc(){
  accD0 = (uint32_t)accumulator[0]; accD1 = (uint32_t)accumulator[1];
}

void _eqD(uint32_t param){loadDFromAcc(); RLO=(accD1==accD0); cancel_RLO=false;}
void _diffD(uint32_t param){loadDFromAcc(); RLO=(accD1!=accD0); cancel_RLO=false;}
void _gtD(uint32_t param){loadDFromAcc(); RLO=(accD1>accD0); cancel_RLO=false;}
void _ltD(uint32_t param){loadDFromAcc(); RLO=(accD1<accD0); cancel_RLO=false;}
void _gteqD(uint32_t param){loadDFromAcc(); RLO=(accD1>=accD0); cancel_RLO=false;}
void _lteqD(uint32_t param){loadDFromAcc(); RLO=(accD1<=accD0); cancel_RLO=false;}

void loadRFromAcc(){
  accR0 = (double)accumulator[0]; accR1 = (double)accumulator[1];
}

void _eqR(uint32_t param){loadRFromAcc(); RLO=(accR1==accR0); cancel_RLO=false;}
void _diffR(uint32_t param){loadRFromAcc(); RLO=(accR1!=accR0); cancel_RLO=false;}
void _gtR(uint32_t param){loadRFromAcc(); RLO=(accR1>accR0); cancel_RLO=false;}
void _ltR(uint32_t param){loadRFromAcc(); RLO=(accR1<accR0); cancel_RLO=false;}
void _gteqR(uint32_t param){loadRFromAcc(); RLO=(accR1>=accR0); cancel_RLO=false;}
void _lteqR(uint32_t param){loadRFromAcc(); RLO=(accR1<=accR0); cancel_RLO=false;}*/

void _ju(uint32_t param){
  addr = param & 0xFF;
  PC = addr;
}

void _jc(uint32_t param){
  addr = param & 0xFF;
  if(RLO == 1)PC = addr;
  cancel_RLO = true;
}

void _jcn(uint32_t param){
  addr = param & 0xFF;
  if(RLO == 0)PC = addr;
  cancel_RLO = true;
}

void _gda(uint32_t param){
  DateTime now = MCP7940.now();
  accumulator[0] = now.day();
}

void _gmo(uint32_t param){
  DateTime now = MCP7940.now();
  accumulator[0] = now.month(); 
}

void _gye(uint32_t param){
  DateTime now = MCP7940.now();
  accumulator[0] = now.year();
}

void _gho(uint32_t param){
  DateTime now = MCP7940.now();
  accumulator[0] = now.hour();
}

void _gmi(uint32_t param){
  DateTime now = MCP7940.now();
  accumulator[0] = now.minute();
}

void _gse(uint32_t param){
  DateTime now = MCP7940.now();
  accumulator[0] = now.second();
}

void _sda(uint32_t param){
  //Serial.print("sda ");Serial.println(accumulator[0]);
  DateTime now = MCP7940.now();
  MCP7940.adjust(DateTime(now.year(),now.month(),(uint8_t)accumulator[0],now.hour(),now.minute(),now.second()));
}

void _smo(uint32_t param){
  DateTime now = MCP7940.now();
  MCP7940.adjust(DateTime(now.year(),(uint8_t)accumulator[0],now.day(),now.hour(),now.minute(),now.second()));
}

void _sye(uint32_t param){
  DateTime now = MCP7940.now();
  MCP7940.adjust(DateTime((uint16_t)accumulator[0],now.month(),now.day(),now.hour(),now.minute(),now.second()));
}

void _sho(uint32_t param){
  DateTime now = MCP7940.now();
  MCP7940.adjust(DateTime(now.year(),now.month(),now.day(),(uint8_t)accumulator[0],now.minute(),now.second()));
}

void _smi(uint32_t param){
  DateTime now = MCP7940.now();
  MCP7940.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),(uint8_t)accumulator[0],now.second()));
}

void _sse(uint32_t param){
  DateTime now = MCP7940.now();
  MCP7940.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),now.minute(),(uint8_t)accumulator[0]));
}
