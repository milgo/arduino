#include <PCF8574.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

#define NUMBER_OF_OPTIONS 4
#define MAX_STRING_SIZE 20

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

#define MAX_PROG_LEN 9
//#define MAX_DATA_LEN 16

#define A 1ULL
#define O 2ULL
#define ASGN 3ULL

#define Q0 1ULL
#define M0 2ULL
#define M1 3ULL

#define s_stll(a1, a2, a3) ((a1<<40ULL) | ((a2) << (32ULL)) | (a3))

uint64_t program[] = {
    s_stll(A, M0, 0),
    s_stll(A, M1, 0),
    s_stll(ASGN, M1, 1),
    s_stll(A, M0, 2),
    s_stll(A, M1, 4),
    s_stll(ASGN, M1, 1),
    s_stll(O, M0, 2),
    s_stll(O, M1, 4),
    s_stll(ASGN, M1, 1)
};

char menuTreeText[NUMBER_OF_OPTIONS][MAX_STRING_SIZE] = 
{
  //Main Menu
  "Run",
  "Edit",
  "Clear",
  "Setup",
};

const char* comStr[] = {"xxx", "A  ", "O  ", "=  "};
const byte comCodes[] = {0x0, 0x1, 0x2, 0x3, 0x4};

const char* memStr[] = {"xx", "Q0", "M0", "M1"};
const byte memCodes[] = {0x0, 0x1, 0x2, 0x3};

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

unsigned int enterMenu(int fromPos, int selectedPos){
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); 
  int pos = selectedPos;
  int toPos = fromPos + 4;
  unsigned char prevButtons = 0, newButtons = 0;
  
  while(true){
    display.clearDisplay();
    
    if(IS_PRESSED(newButtons, BUTTON_ENTER)) return pos;
    if(IS_PRESSED(newButtons, BUTTON_LEFT)) return -1;
    if(pos>fromPos && IS_PRESSED(newButtons, BUTTON_UP)) pos--;
    if(pos<toPos-1 && IS_PRESSED(newButtons, BUTTON_DOWN)) pos++;
    for(int i=fromPos, j=0; i<toPos; i++, j++){
        if(pos == i){
          display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); 
        }
  
        display.setCursor(0,j*8);
        display.println(menuTreeText[i]);
        
        if(pos == i){
          display.setTextColor(SSD1306_WHITE); 
        }
      }
      display.display();

      newButtons = getButtons();
      delay(100);
  }
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

void editProgram(){
  int pl = 0; int j=0;
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); 
  
  while(true){
    display.clearDisplay();
    for(int i=pl; i<pl+4; i++){
      display.setCursor(0, (i-pl)*8);
      uint8_t func_id = program[i] >> FUNC_BIT_POS;
      uint64_t param = program[i] & FUNC_PARAM_MASK;
      int mem_pos = param >> 32;
      int bit_pos = param & 0x7;

      display.print(i);display.print(": ");
      display.print(comStr[func_id]);display.print(" ");
      display.print(memStr[mem_pos]);display.print(" ");
      display.print(bit_pos);
      //display.print("x");
    }
    display.display();
    delay(2000);
    pl++;
    if(pl>5)pl=0;
  }
}

void loop() {

  int newMenuPosition = enterMenu(menuPosition, selectedPosition);

  switch(newMenuPosition){
    case -1: exitCurrentMenu(menuPosition); break;
    case 0: enterCurrentOption(newMenuPosition); break;
    case 1: editProgram(); break;
    case 2: enterCurrentOption(newMenuPosition); break;
    case 3: enterCurrentOption(newMenuPosition); break;
    default:break;
  }
}
