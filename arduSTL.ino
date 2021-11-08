
#include "messages.h"
#include "stl.h"
#include "gui.h"

int PS = 0;

void setup() {
  Serial.begin(9600);

  setupGUI();

  //setupFunctions();
  /*program[0] = s_stll(A, M0, 0);
  program[1] = s_stll(A, M0, 1);
  program[2] = s_stll(ASGN, M1, 1);
  program[3] = s_stll(A, M0, 2);
  program[4] = s_stll(A, M0, 3);
  program[5] = s_stll(ASGN, M1, 1);
  program[6] = s_stll(O, M0, 4);
  program[7] = s_stll(O, M0, 5);
  program[8] = s_stll(ASGN, M1, 1);*/

  program[0] = s_stll_v(L, CS, 6000UL);
  program[1] = s_stll_m(A, M, 0, 0);
  program[2] = s_stll_m(TSF, TIM, 0, 0);
  program[3] = s_stll_m(A, TIM, 0, 0);
  program[4] = s_stll_m(ASGN, Q, 0, 5);
  program[5] = s_stll_m(A, M, 0, 1);
  program[6] = s_stll_m(TRT, TIM, 0, 0);

  PS = 7;

  DDRB = B00100000;//PORTB output pin 5
  PORTD = B11111100;//pullup on pin 2

  //setup global timer
  //------------------
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1=0;

  OCR1A=20000;
  
  TCCR1B|=(1<<CS11)|(1<<WGM12);
  TIMSK1|=(1<<OCIE1A);
  interrupts();
  //------------------
  
  delay(2000);

  /*if(!IS_PRESSED(getButtonsNoneBlocking(), BUTTON_ENTER)){
    if(!IS_PRESSED(getButtonsBlocking(), BUTTON_ENTER))
      delay(100);*/

      boolean conf = true;
      while(conf){
        int newMenuPosition = showMenu(mainMenu, 0, MAIN_MENU_OPTS);
        switch(newMenuPosition){
          case -1: /*exitCurrentMenu(menuPosition);*/ break;
          case 0: /*enterCurrentOption(newMenuPosition);*/conf = false; break;
          case 1: /*enterValue("Enter value:", k, true, 9, 9);break;*/editProgram(); break;
          case 2: /*enterCurrentOption(newMenuPosition);*/ break;
          case 3: /*enterCurrentOption(newMenuPosition);*/ break;
          default:break;
        }
      }

  //}
  
}

ISR(TIMER1_COMPA_vect){
  timersRoutine();
}

void insertProgramLine(int number, bool edit){
  //Serial.print("removing ");Serial.print(number); Serial.print("line");
  int64_t command = 0, mem = -1;
  uint8_t var_pos = 0, bit_pos = 0;
  int8_t comGroup = showMenu(commandGroupMenu, 0, 6);
  long int value = 0;
  if(comGroup>=0){
    command = showMenu(comStr, comGroups[comGroup*2], comGroups[comGroup*2+1]);
    if(command>=0 && memGroups[comGroup*2]>0){

      uint8_t memPtrFrom, memPtrTo;
      memPtrFrom = memGroups[comGroup*2];
      memPtrTo = memGroups[comGroup*2+1];

      if(command == 11){// if transfer command
        memPtrTo -= 1; //exclude constant from load operator
      }
      
      mem = showMenu(memStr, memPtrFrom, memPtrTo);
      if(mem >= 0){

        value = enterValue(memPosAquireMsg[mem], 0, 
                              memValidationRules[mem*4], 
                              memValidationRules[mem*4+1], 
                              memValidationRules[mem*4+2]);

        if(value > memValidationRules[mem*4+3] && memValidationRules[mem*4+3]>0){
          printMessageAndWaitForButton(MUST_BE_LESS_MSG, memValidationRules[mem*4+3]);
          return;
        }
        
        if(mem != CS)//const
          var_pos = value;

        if(mem < 4){
          bit_pos = enterValue(ENTER_BIT_POS_MSG, 0, 0, 1, 7);
        }

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

          if(mem == CS){ //constant
            Serial.print((long)command);Serial.print(" ");Serial.print((long)mem);Serial.print(" ");Serial.print((long)value);Serial.print(" ");
            program[number] = s_stll_v(command, mem, value);
          }
          else{
            Serial.print((long)command);Serial.print(" ");Serial.print((long)mem);Serial.print(" ");Serial.print((long)var_pos);Serial.print(" ");Serial.print((long)bit_pos);Serial.print(" ");
            program[number] = s_stll_m(command, mem, var_pos, bit_pos);
          }
        }
      }
    }
  } 
}

void removeProgramLine(int number){
  Serial.print("removing ");Serial.print(number); Serial.print("line");
  for(int i=number;i<PS;i++){
    program[i] = program[i+1];
  }
  PS--;
}

void editProgram(){
  int pl = 0; int j=0;
  int pos = 0; 
  
  unsigned char newButtons = 0;
  displaySetTextNormal();
  
  while(true){
    displayClear();

    if(IS_PRESSED(newButtons, BUTTON_ENTER) && pos < PS) {
      int res = showMenu(editMenu, 0, 3);
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
      uint64_t param = program[i] & FUNC_PARAM_MASK;
      uint64_t mem_pos = param >> 32 & 0xFF;
      uint64_t var_pos = param >> 4 & 0xFF;
      uint64_t bit_pos = param & 0xF;
      long int value = param & 0xFFFFFFFF;
      if(pos == i){
          displaySetTextInvert();
        }

      if(i<PS){
        displayPrint(i+1);displayPrint(": ");
        //printCommand(func_id);
        printA(comStr, func_id);
        if(mem_pos>0){

          printA(memStr, mem_pos);
          if(mem_pos == CS)//constant
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
  //setupMem();
  displayClear();
  displaySetTextNormal();
  
  //run
  if(program[0]!=0x0){
    printA(message, RUNNING_MSG);
    displayDisplay();
    while(true){
  
      m[0] = ~getButtonsNoneBlocking();
      //delay(100);
      
      executeCommandAt(PC++);
      if(PC>=PS)
        PC=0;
    }
  }else{
    printA(message, NOPROGRAM_MSG);
    displayDisplay();
    delay(3000);
  }
}

void loop() {
  runProgram();
  
}
