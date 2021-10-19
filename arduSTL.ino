
#include "messages.h"
#include "stl.h"
#include "gui.h"

int PS = 0;

void setup() {
  Serial.begin(9600);

  setupGUI();

  //setupFunctions();
  program[0] = s_stll(A, M0, 0);
  program[1] = s_stll(A, M1, 0);
  program[2] = s_stll(ASGN, M1, 1);
  program[3] = s_stll(A, M0, 2);
  program[4] = s_stll(A, M1, 4);
  program[5] = s_stll(ASGN, M1, 1);
  program[6] = s_stll(O, M0, 2);
  program[7] = s_stll(O, M1, 4);
  program[8] = s_stll(ASGN, M1, 1);

  PS = 9;

  delay(2000);
}

void insertProgramLine(int number, bool edit){
  //Serial.print("removing ");Serial.print(number); Serial.print("line");
  int64_t command = 0, mem = 0;
  int64_t value = 0;
  int comGroup = showMenu(commandGroupMenu, 0, 6);
  if(comGroup>=0){
    command = showMenu(comStr, comGroups[comGroup*2], comGroups[comGroup*2+1]);
    if(command>=0 && memGroups[comGroup*2]>0){
      mem = showMenu(memStr, memGroups[comGroup*2], memGroups[comGroup*2+1]);
      if(mem < 7 && mem > 0){
        value = enterValue(ENTER_BIT_NR_MSG, 0, false, 1, 7);
      }else if(mem >= 7 && mem < 10){
        value = enterValue(ENTER_VALUE_MSG, 0, false, 2, 9);
      }else if(mem == 10){
        value = enterValue(ENTER_VALUE_MSG, 0, true, 9, 9);
      }else if(mem > 10){
        value = enterValue(ENTER_VALUE_MSG, 0, false, 1, 9);
      }
      if(mem >= 0){
        if(PS>=MAX_PROGRAM_SIZE){
          printMessageAndWaitForButton(LIMIT_MSG);
        }else {
          if(!edit){
            if(number < PS){        
              for(int i=number;i<PS;i++){
                program[PS-i+1] = program[PS-i];
              }
            }
          Serial.print((long)command);Serial.print(" ");Serial.print((long)mem);Serial.print(" ");Serial.print((long)value);Serial.print(" ");
            program[number] = s_stll(command, mem, value);
            if(PS<MAX_PROGRAM_SIZE)PS++;
          }else{
            program[number] = s_stll(command, mem, value);
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
      uint64_t mem_pos = param >> 32;
      uint64_t bit_pos = param & 0xFFFFFFFF;

      if(pos == i){
          displaySetTextInvert();
        }

      if(i<PS){
        displayPrint(i);displayPrint(": ");

        //printCommand(func_id);
        printA(comStr, func_id);

        if(mem_pos>0){
          printA(memStr, mem_pos);
          displayPrint((long)bit_pos);displayPrint(" ");
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
    newButtons = getButtons();
    delay(100);
    //pl++;
    //if(pl>5)pl=0;
  }
}

void runProgram(){
  //load
  
  unsigned char newButtons = 0;
  setupMem();//!!!!!!!!!!!!!!!!!!!!
  displayClear();
  displaySetTextNormal();
  printA(message, RUNNING_MSG);
  displayDisplay();
  //run
  while(true){
    newButtons = getButtons();

    if(IS_PRESSED(newButtons, BUTTON_LEFT))
      break;
    
    executeCommand(PC++);
    if(PC>=PS)
      PC=0;
  }
}

void loop() {

  int newMenuPosition = showMenu(mainMenu, 0, MAIN_MENU_OPTS);
  switch(newMenuPosition){
    case -1: /*exitCurrentMenu(menuPosition);*/ break;
    case 0: /*enterCurrentOption(newMenuPosition);*/runProgram(); break;
    case 1: /*enterValue("Enter value:", k, true, 9, 9);break;*/editProgram(); break;
    case 2: /*enterCurrentOption(newMenuPosition);*/ break;
    case 3: /*enterCurrentOption(newMenuPosition);*/ break;
    default:break;
  }
}
