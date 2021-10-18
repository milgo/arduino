#include <PCF8574.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "inc.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
PCF8574 pcf20(0x20);

#define MAIN_MENU_OPTS 4
#define COMM_MENU_OPTS 6
#define MAX_STRING_SIZE 10
#define MAX_PROGRAM_SIZE 10

#define FUNC_BIT_POS 40
#define FUNC_PARAM_MASK 0xFFFFFFFFFFULL

#define BUTTON_DOWN 0
#define BUTTON_LEFT 2
#define BUTTON_RIGHT 1
#define BUTTON_UP 3
#define BUTTON_ENTER 4
#define IS_PRESSED(BUTTONS, BUTTON) (((BUTTONS) & (1<<(BUTTON))) == (1<<(BUTTON)))

int menuPosition = 0;
int selectedPosition = 0;

//#define MAX_PROG_LEN 9
//#define MAX_DATA_LEN 16

#define A 1ULL
#define O 2ULL
#define ASGN 3ULL

#define Q0 1ULL
#define M0 2ULL
#define M1 3ULL

#define s_stll(a1, a2, a3) ((a1<<40ULL) | ((a2) << (32ULL)) | (a3))

uint64_t program[MAX_PROGRAM_SIZE];
char PC = 0;

unsigned char getButtons(){
  while(true){
    for(int i=0; i<8; i++){
      int buttonStatus = pcf20.readButton(i);
      if(!buttonStatus){
        while(true){
          buttonStatus = pcf20.readButton(i);
          if(buttonStatus)
            return (1<<i);
        }
      }
    }
  }
  return 0x0;
}

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  //display.drawPixel(10, 10, SSD1306_WHITE);

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

  PC = 9;

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  //drawFunctionBlock(func[0]);
  delay(2000);
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(20000);
}
void enterCurrentOption(int newMenuPosition){
  menuPosition = (newMenuPosition + 1) * 4;
  selectedPosition = menuPosition;
  Serial.println(menuPosition);
}

void exitCurrentMenu(int currentMenuPos){
  
  menuPosition = (currentMenuPos) / 4 - 1;// / 4 - modulo;
  int modulo = menuPosition % 4;
  menuPosition -= modulo;
  if(menuPosition<0)menuPosition=0;
  if(modulo<0)modulo=0;
  selectedPosition = menuPosition + modulo;
  //menuPosition = menuPosition * 4;
  Serial.println(modulo);
}

void selectMemmory(){
  
}

int showMenu(const char * const *menu, int from, int to){
  int pos = 0, start = 0; int len = to - from;
  unsigned char newButtons = 0;
  while(true){
    display.clearDisplay();
    display.setCursor(0, 0);

    if(IS_PRESSED(newButtons, BUTTON_LEFT)) return -1;
    if(IS_PRESSED(newButtons, BUTTON_ENTER)) return from+pos;
    if(pos>0 && IS_PRESSED(newButtons, BUTTON_UP)) pos--;
    if(pos<len-1 && IS_PRESSED(newButtons, BUTTON_DOWN)) pos++;

    if(pos<start && start>0)start--;
    else if(pos>start+3 && start<len-4)start++;

    for(int i=start; i<start+4; i++){

      if(from+i>=to)break;
      
      if(pos == i){
          display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); 
      }
  
      display.setCursor(0, (i-start)*8);

      strcpy_P(bufferStr, (char*)pgm_read_word(&(menu[from+i])));
      display.println(bufferStr);
        
      if(pos == i){
        display.setTextColor(SSD1306_WHITE); 
      }
    }

    display.display();
    newButtons = getButtons();
    delay(100);
  }
}

long int enterValue(int msg, long int curVal, bool isSigned, int len, int maxDigit){
  unsigned char newButtons = 0;
  long int retVal = 0;
  byte v[10];

  //get sign
  v[0]=0;
  if(curVal<0L){
    if(isSigned)v[0]=1;
    curVal*=-1;
  }

  //convert to array
  long int tmp = curVal;
  for(int j=len;j>0;j--){
    v[j]=tmp%10;
    tmp/=10;
  }

  //edit
  int pos = len;
  int maxVal = 1;
  while(true){
    display.clearDisplay();
    display.setCursor(0, 0);

    strcpy_P(bufferStr, (char*)pgm_read_word(&(message[msg])));
    display.print(bufferStr);
    
    if(pos>0 && IS_PRESSED(newButtons, BUTTON_LEFT)) pos--;
    if(pos<len && IS_PRESSED(newButtons, BUTTON_RIGHT)) pos++;

    if(pos>0){maxVal = maxDigit;}
    else {
      if(isSigned)maxVal = 1;
      else maxVal = 0;
    }

    if(IS_PRESSED(newButtons, BUTTON_UP)){ if(v[pos]<maxVal)v[pos]++; }
    if(IS_PRESSED(newButtons, BUTTON_DOWN)){ if(v[pos]>0)v[pos]--; }
    if(IS_PRESSED(newButtons, BUTTON_ENTER)){ break; }
    
    for(int i=0; i<len+1; i++){
      
      if(pos==i){
        display.setCursor(i*6, 16);
        display.print("^ ");
        //display.print("]");
      }

      display.setCursor(i*6, 8);
      if(i==0){
        if(v[0]==0)display.print("+");
        else display.print("-");
      }else{
        display.print(v[i]);
      }
    }

    display.display();
    newButtons = getButtons();
    delay(100);
  }

  display.clearDisplay();
  display.setCursor(0, 0);

  //convert from array
  for(int j=1;j<len+1;j++){
    retVal *= 10;
    retVal += v[j];
  }

  //set sign
  if(v[0]>0)retVal*=-1;

  //Serial.print("retVal=");
  //Serial.println(retVal);

  return retVal;
}

void printMessage(int msg){
  unsigned char newButtons = 0;
  while(true){
    display.clearDisplay();
    strcpy_P(bufferStr, (char*)pgm_read_word(&(message[msg])));
    display.print(bufferStr);
    display.display();
    newButtons = getButtons();
    if(IS_PRESSED(newButtons, BUTTON_ENTER))
      break;
  }
  display.clearDisplay();
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
        if(PC>=MAX_PROGRAM_SIZE){
          printMessage(LIMIT_MSG);
        }else {
          if(!edit){
            if(number < PC){        
              for(int i=number;i<PC;i++){
                program[PC-i+1] = program[PC-i];
              }
            }
          Serial.print((long)command);Serial.print(" ");Serial.print((long)mem);Serial.print(" ");Serial.print((long)value);Serial.print(" ");
            program[number] = s_stll(command, mem, value);
            if(PC<MAX_PROGRAM_SIZE)PC++;
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
  for(int i=number;i<PC;i++){
    program[i] = program[i+1];
  }
  PC--;
}

void editProgram(){
  int pl = 0; int j=0;
  int pos = 0; 
  
  unsigned char newButtons = 0;
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); 
  
  while(true){
    display.clearDisplay();

    

    if(IS_PRESSED(newButtons, BUTTON_ENTER) && pos < PC) {
      int res = showMenu(editMenu, 0, 3);
      switch(res){
        case 0: insertProgramLine(pos, false);break;
        case 1: insertProgramLine(pos, true);break;
        case 2: removeProgramLine(pos);if(pos>0)pos--;if(pl>0)pl--;break;
        default: break;
      }
    }else if(IS_PRESSED(newButtons, BUTTON_ENTER) && pos == PC && PC<MAX_PROGRAM_SIZE) {
      insertProgramLine(pos, false);
    }
    
    if(IS_PRESSED(newButtons, BUTTON_LEFT)) return;
    if(pos>0 && IS_PRESSED(newButtons, BUTTON_UP)) pos--;
    if(pos<PC && IS_PRESSED(newButtons, BUTTON_DOWN))pos++;

    if(pos<pl && pl>0)pl--;
    else if(pos>pl+3 && pl<PC-3)pl++;

    //Serial.print(pos); Serial.print(", "); Serial.println(pl);
    
    for(int i=pl; i<pl+4 && i<=PC; i++){
      display.setCursor(0, (i-pl)*8);
      uint8_t func_id = program[i] >> FUNC_BIT_POS;
      uint64_t param = program[i] & FUNC_PARAM_MASK;
      uint64_t mem_pos = param >> 32;
      uint64_t bit_pos = param & 0xFFFFFFFF;

      if(pos == i){
          display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); 
        }

      if(i<PC){
        display.print(i);display.print(": ");

        strcpy_P(bufferStr, (char*)pgm_read_word(&(comStr[func_id])));
        display.print(bufferStr);display.print(" ");

        if(mem_pos>0){
          strcpy_P(bufferStr, (char*)pgm_read_word(&(memStr[mem_pos])));
          display.print(bufferStr);display.print(" ");
          display.print((long)bit_pos);display.print(" ");
        }
      }else {
        if(PC<MAX_PROGRAM_SIZE)
          display.print("[+]");
        else
          display.print("LIMIT");
      }

      if(pos == i){
          display.setTextColor(SSD1306_WHITE); 
        }
    }

    display.display();
    newButtons = getButtons();
    delay(100);
    //pl++;
    //if(pl>5)pl=0;
  }
}

void loop() {

  int newMenuPosition = showMenu(mainMenu, 0, MAIN_MENU_OPTS);
  long int k = -123456789;
  switch(newMenuPosition){
    case -1: exitCurrentMenu(menuPosition); break;
    case 0: enterCurrentOption(newMenuPosition); break;
    case 1: /*enterValue("Enter value:", k, true, 9, 9);break;*/editProgram(); break;
    case 2: enterCurrentOption(newMenuPosition); break;
    case 3: enterCurrentOption(newMenuPosition); break;
    default:break;
  }
}
