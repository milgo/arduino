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

#define NUMBER_OF_OPTIONS 20
#define MAX_STRING_SIZE 20

#define BUTTON_DOWN 0
#define BUTTON_LEFT 2
#define BUTTON_RIGHT 1
#define BUTTON_UP 3
#define BUTTON_ENTER 4
#define IS_PRESSED(BUTTONS, BUTTON) (((BUTTONS) & (1<<(BUTTON))) == (1<<(BUTTON)))

int menuPosition = 0;
int selectedPosition = 0;
char menuTreeText[NUMBER_OF_OPTIONS][MAX_STRING_SIZE] = 
{
  //Main Menu
  "Program",
  "Card",
  "Setup",
  "Start",

  //Program
  "Edit",
  "Clear Prg",
  "Password",
  "Msg Config",

  //Card
  "Card1",
  "Card2",
  "Card3",
  "Card4",

  //Setup
  "Setup1",
  "Setup2",
  "Setup3",
  "Setup4",

  //Clock
  "Start1",
  "Start2",
  "Start3",
  "Start4"
  
};

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

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
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

void loop() {

  int newMenuPosition = enterMenu(menuPosition, selectedPosition);

  switch(newMenuPosition){
    case -1: exitCurrentMenu(menuPosition); break;
    case 0: enterCurrentOption(newMenuPosition); break;
    case 1: enterCurrentOption(newMenuPosition); break;
    case 2: enterCurrentOption(newMenuPosition); break;
    case 3: enterCurrentOption(newMenuPosition); break;
    default:break;
  }

  
}
