///////////////////////////////////////////////////////////////////////////////////////////////
// Title: Flow OS v1                                                                         //
// Purpose: To create an operating system for Palmer Technologies' Flow Extruder Prototype.  //
// Developer: Cameron Palmer                                                                 //
// Last Modified: August 9, 2020                                                             //
///////////////////////////////////////////////////////////////////////////////////////////////

#define PROTOTYPE_VERSION "1.0"

// Included Libraries
#include <AccelStepper.h> // For motors
#include <LiquidCrystal.h> // For LCD
#include <LiquidMenu.h> // For menus on the LCD
#include <Encoder.h> // For rotary encoder
#include <PID_v1.h> // For PID temperature control

#include <Thermocouple.h>             // Thermocouple libraries
#include <MAX6675_Thermocouple.h>     //

//==================================

// Pinout Constants
#define FAN 3
#define HEATER 4

// Thermocouple Constants
#define THERMO_SCK 5
#define THERMO_CS 6
#define THERMO_SO 7

//Interface Constants
#define DB4 8
#define DB5 9
#define DB6 10
#define DB7 11
#define RS 12
#define ENABLE 13
#define ENC_CLOCK 14
#define ENC_DATA 15
#define ENC_SWITCH 16

//Motor Constants
#define M_EXTRUDER_STEP 46
#define M_EXTRUDER_DIR 47
#define M_ROLLER_STEP 48
#define M_ROLLER_DIR 49
#define M_LEVEL_STEP 50
#define M_LEVEL_DIR 51
#define M_WINDER_STEP 52
#define M_WINDER_DIR 53

//==================================

//Objects
// Stepper objects
AccelStepper m_extruder(1, M_EXTRUDER_STEP, M_EXTRUDER_DIR);
AccelStepper m_roller(1, M_ROLLER_STEP, M_ROLLER_DIR);
AccelStepper m_level(1, M_LEVEL_STEP, M_LEVEL_DIR);
AccelStepper m_winder(1, M_WINDER_STEP, M_WINDER_DIR);

// Thermocouple Object
Thermocouple* thermocouple = new MAX6675_Thermocouple(THERMO_SCK, THERMO_CS, THERMO_SO);

// LCD Object
LiquidCrystal lcd(RS, ENABLE, DB4, DB5, DB6, DB7);

// Encoder Object & Variables
Encoder enc(ENC_CLOCK, ENC_DATA);
long encOldPosition = -999;
long newPosition = 0;
unsigned long currentTime = 0;
//==================================

// Status display variables
short extruderMotorStatus = 0;
short rollerMotorStatus = 0;
short levelMotorStatus = 0;
short winderMotorStatus = 0;

// PID variables, PID object
double temperature = -99;
double newTemp = 0;
double setTemp = 50;
double output;
double Kp = 2, Ki = 5, Kd = 1;
unsigned long pidTimekeeper = 0;
PID pid(&temperature, &output, &setTemp, Kp, Ki, Kd, DIRECT); 

// Used for attaching to LiquidLines to make them focusable, and thus scrollable
void blankFunction() {
  return;
  
 }

// Menu creation
LiquidMenu menu(lcd);

LiquidLine welcomeLine0(0,0, "Flow Extruder");
LiquidLine welcomeLine1(0,1, "Prototype v", PROTOTYPE_VERSION); 
LiquidScreen welcomeScreen(welcomeLine0, welcomeLine1); // Welcome screen

LiquidLine statusLine0(0,0, temperature, "C");
LiquidLine statusLine1(0,1,"Click for more");
LiquidScreen statusScreen(statusLine0, statusLine1); // Status screen. Shows temp of heater.

LiquidLine optionsLine0(0,0, " Back"); // On screens with selectable elements, like the options screen, leave a space at the front of the string for the focus symbol
LiquidLine optionsLine1(0,1, " Set temp (", setTemp, "C)");
LiquidLine optionsLine2(0,1, " Set ext spd");
LiquidLine optionsLine3(0,1, " Set rol spd");
LiquidLine optionsLine4(0,1, " Set lvl spd");
LiquidLine optionsLine5(0,1, " Set wind spd"); // Options screen to change set temp, motor speeds, eventually (in prototype v2?) filament diameter
LiquidScreen optionsScreen;


void setup() {
  
  // Start serial monitor for debugging purposes
  Serial.begin(9600);
  
  // Pinmodes
  pinMode(FAN, OUTPUT);
  pinMode(HEATER, OUTPUT);


  // Set up lcd
  lcd.begin(16,2);
  optionsScreen.add_line(optionsLine0);
  optionsScreen.add_line(optionsLine1);
  optionsScreen.add_line(optionsLine2);
  optionsScreen.add_line(optionsLine3);
  optionsScreen.add_line(optionsLine4); // Adding the 5th and 6th lines to the options screen
  optionsScreen.add_line(optionsLine5);

  // Attach functions to options lines
  optionsLine0.attach_function(1, blankFunction);
  optionsLine1.attach_function(1, blankFunction);
  optionsLine2.attach_function(1, blankFunction);
  optionsLine3.attach_function(1, blankFunction);
  optionsLine4.attach_function(1, blankFunction);
  optionsLine5.attach_function(1, blankFunction);

  // More menu setup: show welcome screen, then delay 3s, show status screen
  optionsScreen.set_displayLineCount(2); // Set the number of lines the display has; this is necessary to allow scrolling
  menu.add_screen(welcomeScreen); // Add the screens to the menu
  menu.add_screen(statusScreen);
  menu.add_screen(optionsScreen);
  menu.set_focusPosition(Position::LEFT);
  menu.update();
  delay(3000);
  menu.next_screen();
  

 //analogWrite(HEATER, 100);

  // Start PID (TESTING PURPOSES)
  pid.SetMode(AUTOMATIC);
 
}

void loop() {

  if (millis() >= currentTime + 1000){
    Serial.println(thermocouple->readCelsius());
    currentTime = millis();
    }
  // Motor tests
  //m_extruder.runSpeed();
  //m_winder.runSpeed();
  //m_level.moveTo(100);
  //m_level.run();

  

  // Reading encoder turns
  newPosition = enc.read()/4;
  if ((newPosition != encOldPosition) && (menu.get_currentScreen() != &statusScreen)) {
    if(menu.get_currentScreen() == &optionsScreen && newPosition > encOldPosition){
     menu.switch_focus(false); // Focus backwards (up the screen) when encoder is turned counter-clockwise
    }
    if(menu.get_currentScreen() == &optionsScreen && newPosition < encOldPosition){
      menu.switch_focus(true); // Focus forwards (down the screen) when encoder is turned clockwise
    }
    encOldPosition = newPosition;
    menu.update();
  }


  // Reading encoder button
  if(digitalRead(ENC_SWITCH) == LOW && (currentTime + 1000 <= millis())){
    
   // If current screen is the status screen, go to the options screen
   if(menu.get_currentScreen() == &statusScreen){
      menu.change_screen(&optionsScreen);
      menu.update();
      //delay(500);
    } 
    // If current screen is the options screen, and the selected line is "Back"
    else if(menu.get_currentScreen() == &optionsScreen && menu.get_focusedLine() == 0){
      menu.change_screen(&statusScreen);
      menu.update();
    }
   }

  // Reading thermocouple, storing value in short temperature if value needs to be updated, and updating menu if needed
  if (millis() >= pidTimekeeper + 500){
    newTemp = thermocouple->readCelsius();
    if(temperature != newTemp){
       temperature = newTemp;
        if (menu.get_currentScreen() == &statusScreen){
          menu.update();
      }
    }
    pidTimekeeper = millis();
  }
  

   // Adjusting PID
   pid.Compute();
   analogWrite(HEATER, output);

  
   
  
}
