
#include <EEPROM.h>
#include "messages.h"
#include "stl.h"
#include "gui.h"

boolean programChanged = 1;

#define SCREEN_SAVER_TIME 60
#define EXIT_RUNNING_TIME 6
#define EXIT_RUNNNING_BUTTONS(BUTTONS) IS_PRESSED(BUTTONS, BUTTON_LEFT) && IS_PRESSED(BUTTONS, BUTTON_RIGHT)

void writeProgramToEeprom(){
  printA(message, PROGRAMMING_EEPROM);
  displayDisplay();
  int addr = 0;
  for(uint8_t i=0; i<PS; i++){
    EEPROM.write(addr, (program[i] >> 24) & 0xFF);
    EEPROM.write(addr+1, (program[i] >> 16) & 0xFF);
    EEPROM.write(addr+2, (program[i] >> 8) & 0xFF);
    EEPROM.write(addr+3, program[i] & 0xFF);
    addr+=4;
  }
  //Write PS and the end of eeprom
  EEPROM.write(0x3FF, PS);
  programChanged = 1;
  delay(1000);
}

void readProgramFromEeprom(){
  int addr = 0;
  PS = EEPROM.read(0x3FF);
  if(PS == 0xFF)
    PS = 0;
  //Serial.println(PS);
  for(uint8_t i=0; i<PS; i++){
    program[i] = ((uint32_t)EEPROM.read(addr) << 24UL) +
                  ((uint32_t)EEPROM.read(addr+1) << 16UL)+
                  ((uint32_t)EEPROM.read(addr+2) << 8UL) +
                  ((uint32_t)EEPROM.read(addr+3));
    addr+=4;
  }
  programChanged = 1;
}

void clearProgramLocal(){
  for(uint8_t i=0; i<PS; i++){
      program[i] = 0;
  }
  programChanged = 0;
  PS = 0;
}

int askToSaveChangesIfMade(){
  if(programChanged == 0){
    if(IS_PRESSED(printMessageAndWaitForButton(SAVE_CHANGES), BUTTON_ENTER)){
      writeProgramToEeprom();
      return 0;
    }
    else return 1;
  }
  return 0;
}

void setup() {
  Serial.begin(9600);
  //EEPROM.write(0x3FF, 0xFF);
  setupGUI();


  DDRB = B11111111;//PORTD output
  PORTD = B00000000;
  PORTD = B11111111;//pullup
  //PORTB = B00010010;//pullup
  
  //setup global timer
  //------------------
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1=0;

  OCR1A=20000;
  //OCR1A=2500;
  
  TCCR1B|=(1<<CS11)|(1<<WGM12);
  TIMSK1|=(1<<OCIE1A);
  interrupts();
  //------------------
  readProgramFromEeprom();

  //If enter pressed after reset then go to menu
  /*if(!IS_PRESSED(getButtonsNoneBlocking(), BUTTON_ENTER)){
    if(!IS_PRESSED(getButtonsBlocking(), BUTTON_ENTER))
      delay(100);*/
}

const char _0dot[] PROGMEM = {""};
const char _1dot[] PROGMEM = {"."};
const char _2dot[] PROGMEM = {".."};
const char _3dot[] PROGMEM = {"..."};
const char* const runningPromptArray[] PROGMEM = {_0dot, _1dot, _2dot, _3dot};

uint8_t exitRunningCounter;
uint8_t screenSaverCounter;
uint8_t runningIndCounter;
uint8_t runningIndCounterPrev;

void runEvery500ms(){
  runningIndCounter++;
  if(screenSaverCounter<SCREEN_SAVER_TIME)
    screenSaverCounter++;
  if(EXIT_RUNNNING_BUTTONS(buttons))
    exitRunningCounter--;
  else{
    exitRunningCounter = EXIT_RUNNING_TIME;
  }
}

int timerCounter = 0;
ISR(TIMER1_COMPA_vect){
  timersRoutine();
  timerCounter++;
  if(timerCounter>=50){
    runEvery500ms();
    timerCounter = 0;
  }
}


void insertProgramLine(int number, bool edit){
  //Serial.print("removing ");Serial.print(number); Serial.print("line");
  int32_t command = 0, mem = -1;
  uint8_t var_pos = 0, bit_pos = 0;
  int8_t comGroup = showMenu(commandGroupMenu, NULL, 0, COMM_MENU_SIZE);
  int32_t value = 0;
  if(comGroup>=0){
    command = showMenu(comStr, comDesc, comGroups[comGroup*2], comGroups[comGroup*2+1]);

    uint8_t memPtrFrom, memPtrTo;
    memPtrFrom = pgm_read_byte(&(memGroups[command*2]));
    memPtrTo = pgm_read_byte(&(memGroups[command*2+1]));
    
    if(command>=0 && memPtrFrom>=0){

      /*if(command == 11){// if transfer or persist command
        memPtrTo -= 1; //exclude constant from load operator
      }*/

      if(memPtrFrom>0){ //if operation with mem selection
        mem = showMenu(memStr, memDesc, memPtrFrom, memPtrTo);
        if(mem >= 0){

          char sig = pgm_read_word(&memValidationRules[mem*5]);
          char len = pgm_read_word(&memValidationRules[mem*5+1]);
          char dig = pgm_read_word(&memValidationRules[mem*5+2]);
          int16_t minimum = pgm_read_word(&memValidationRules[mem*5+3]);
          int16_t maximum = pgm_read_word(&memValidationRules[mem*5+4]);
  
          value = enterValue(memPosAquireMsg[mem], 0, sig, len, dig);
          
          if(value < minimum || value > maximum){
            printMessageAndWaitForButton(MUST_BE_IN_RANGE, minimum, maximum);
            return;
          }
          
          if(mem != CS && mem != AD)//const
            var_pos = value;
  
          if(mem < 4){
            bit_pos = enterValue(ENTER_BIT_POS_MSG, 0, 0, 1, 7);
          }
  
        }
      }else{
        mem = 0;
      }

      if(mem >= 0){
        if(PS>=MAX_PROGRAM_SIZE){
          printMessageAndWaitForButton(LIMIT_MSG);
        }else {
          if(!edit){
            if(number < PS){        
              for(int i=PS;i>number;i--){
                program[i] = program[i-1];
              }
            }
            if(PS<MAX_PROGRAM_SIZE)PS++;
          }

          if(mem == CS || mem == AD){ //constant
            //Serial.print((long)command);Serial.print(" ");Serial.print((long)mem);Serial.print(" ");Serial.print((long)value);Serial.print(" ");
            program[number] = s_stll_v(command, mem, value);
            programChanged = 0;
          }
          else{
            //Serial.print((long)command);Serial.print(" ");Serial.print((long)mem);Serial.print(" ");Serial.print((long)var_pos);Serial.print(" ");Serial.print((long)bit_pos);Serial.print(" ");
            program[number] = s_stll_m(command, mem, var_pos, bit_pos);
            //Serial.println(mem);
            programChanged = 0;
          }
        }
      }
    }
  } 
}

void removeProgramLine(int number){
  //Serial.print("removing ");Serial.print(number); Serial.print("line");
  for(int i=number;i<PS;i++){
    program[i] = program[i+1];
  }
  PS--;
  programChanged = 0;
}

void editProgram(){
  int pl = 0; int j=0;
  int pos = 0; 
  
  unsigned char newButtons = 0;
  displaySetTextNormal();
  
  while(true){
    displayClear();

    if(IS_PRESSED(newButtons, BUTTON_ENTER) && pos < PS) {
      int res = showMenu(editMenu, NULL, 0, 3);
      switch(res){
        case 0: insertProgramLine(pos, false);break;
        case 1: insertProgramLine(pos, true);break;
        case 2: removeProgramLine(pos);if(pos>0)pos--;if(pl>0)pl--;break;
        default: break;
      }
    }else if(IS_PRESSED(newButtons, BUTTON_ENTER) && pos == PS && PS<MAX_PROGRAM_SIZE) {
      insertProgramLine(pos, false);
    }
    
    if(IS_PRESSED(newButtons, BUTTON_LEFT)) return;
    if(pos>0 && IS_PRESSED(newButtons, BUTTON_UP)) pos--;
    if(pos<PS && IS_PRESSED(newButtons, BUTTON_DOWN))pos++;

    if(pos<pl && pl>0)pl--;
    else if(pos>pl+3 && pl<PS-3)pl++;

    //Serial.print(pos); Serial.print(", "); Serial.println(pl);
    
    for(int i=pl; i<pl+4 && i<=PS; i++){
      displaySetCursor(0, (i-pl)*8);
      uint8_t func_id = program[i] >> FUNC_BIT_POS;
      uint16_t param = program[i] & FUNC_PARAM_MASK;
      uint8_t mem_pos = (program[i] >> MEM_BIT_POS) & 0xFF;
      uint8_t var_pos = param >> 4 & 0xFF;
      uint8_t bit_pos = param & 0xF;
      int value = param & FUNC_PARAM_MASK;

      if(pos == i){
          displaySetTextInvert();
        }

      if(i<PS){
        displayPrint(i+1);displayPrint(": ");
        //printCommand(func_id);
        printA(comStr, func_id);
        displayPrint(" ");
        if(mem_pos>0){

          printA(memStr, mem_pos);
          if(mem_pos == CS || mem_pos == AD)//constant
            displayPrint((long int)value);
          else
            displayPrint((long)var_pos);
          if(mem_pos<4){//if basic command
            displayPrint(".");
            displayPrint((long)bit_pos);
          }
        }
      }else {
        if(PS<MAX_PROGRAM_SIZE){
          printA(message, ADD_LINE_MSG);
        }
        else{
          printA(message, LIMIT_MSG);
        }
      }

      if(pos == i){
          displaySetTextNormal();
        }
    }

    displayDisplay();
    newButtons = getButtonsBlocking();
    delay(100);
    //pl++;
    //if(pl>5)pl=0;
  }
}

void runProgram(){
  //load
  setupMem();
  displayClear();
  displaySetTextNormal();
  
  runningIndCounter = 0;
  runningIndCounterPrev = 0;
  screenSaverCounter = 0;
  exitRunningCounter = EXIT_RUNNING_TIME;
  
  //run
  if(program[0]!=0x0){
    //----
    
    while(exitRunningCounter>0){

      readAnalog();
      writeAnalog();
      
      displayClear();
      if(screenSaverCounter < 60){

        //display running indicator
        if(runningIndCounterPrev != runningIndCounter){
          
          displaySetCursor(0, 0);
          printA(message, RUNNING_MSG);
          printA(runningPromptArray, runningIndCounter%4);
  
          //display values
          uint8_t lines = 0;
          int16_t value;
          if(m[1] & (1 << 4)){value = m[58] + (m[59] << 8); lines++;;displaySetCursor(0, lines*8);displayPrint(lines);printA(message, COLON); displayPrint(value);}
          if(m[1] & (1 << 5)){value = m[60] + (m[61] << 8); lines++;displaySetCursor(0, lines*8);displayPrint(lines);printA(message, COLON); displayPrint(value);}
          if(m[1] & (1 << 6)){value = m[62] + (m[63] << 8); lines++;displaySetCursor(0, lines*8);displayPrint(lines);printA(message, COLON); displayPrint(value);}
          
          displayDisplay();
          runningIndCounterPrev = runningIndCounter;
        }
      }
      else{
        displayDisplay();
      }

      uint8_t newButtons = ~getButtonsNoneBlocking();

      if(screenSaverCounter<60)
        buttons = newButtons;
        
      if(buttons != newButtons)
        screenSaverCounter=0;
        
      
      //delay(100);
      
      executeCommandAt(PC++);
      if(PC>=PS){
        afterFirstScan();
        PC=0;
      }
    }
  }else{
    displayClear();
    displaySetCursor(0, 0);
    printA(message, NOPROGRAM_MSG);
    displayDisplay();
    delay(3000);
  }
}

int newMenuPosition = -2;

void loop() {
  
  //while(conf){
      switch(newMenuPosition){
        case -1: break;
        case 0: if(askToSaveChangesIfMade()==0){runProgram();} break;
        case 1: editProgram(); break;
        case 2: if(programChanged==0)writeProgramToEeprom();else printMessageAndWaitForButton(NO_CHANGES);break;
        case 3: clearProgramLocal(); break;
        default: runProgram(); break;
      }
      newMenuPosition = showMenu(mainMenu, NULL, 0, MAIN_MENU_SIZE);
  //}

  //}
  
}
