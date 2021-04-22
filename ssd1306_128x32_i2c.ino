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

#define NUMBER_OF_STRING 4
#define MAX_STRING_SIZE 40

#define BUTTON_DOWN 0
#define BUTTON_LEFT 2
#define BUTTON_RIGHT 1
#define BUTTON_UP 3
#define BUTTON_ENTER 4
#define IS_PRESSED(BUTTONS, BUTTON) (((BUTTONS) & (1<<(BUTTON))) == (1<<(BUTTON)))

char mainMenuText[NUMBER_OF_STRING][MAX_STRING_SIZE] = 
{
  "Program",
  "Card",
  "Setup",
  "Start"
};

char editMenuText[NUMBER_OF_STRING][MAX_STRING_SIZE] = 
{
  "Edit",
  "Clear Prg",
  "Password",
  "Msg Config"
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

unsigned int drawMenu(char menu[][MAX_STRING_SIZE]){
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); 
  int menuPosition = 0;
  unsigned char prevButtons = 0, newButtons = 0;
  
  while(true){
    display.clearDisplay();
    
    //if(prevButtons != newButtons){
      //prevButtons = newButtons;
      if(IS_PRESSED(newButtons, BUTTON_ENTER)) return menuPosition;
      if(IS_PRESSED(newButtons, BUTTON_LEFT)) return -1;
      if(menuPosition>0 && IS_PRESSED(newButtons, BUTTON_UP)) menuPosition--;
      if(menuPosition<NUMBER_OF_STRING-1 && IS_PRESSED(newButtons, BUTTON_DOWN)) menuPosition++;
    //}

    for(int i=0; i<NUMBER_OF_STRING; i++){
        if(menuPosition == i){
          display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); 
        }
  
        display.setCursor(0,i*8);
        display.println(menu[i]);
        
        if(menuPosition == i){
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

void loop() {

  //Serial.println(F("waiting..."));
  int selectedMainMenuOption = drawMenu(mainMenuText);
  switch(selectedMainMenuOption){
    //case -1:break;
    case 0: {
      
      int selectedEditMenuOption = drawMenu(editMenuText);
      switch(selectedEditMenuOption){
        case -1:break;
        case 0:break;
        case 1:break;
        case 2:break;
        case 3:break;
        default: break;
      }
      break;
    }
    case 1: break;
    case 2: break;
    case 3: break;
    default:break;
  }
  
}
