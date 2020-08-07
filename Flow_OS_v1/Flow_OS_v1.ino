///////////////////////////////////////////////////////////////////////////////////////////////
// Title: Flow OS v1                                                                         //
// Purpose: To create an operating system for Palmer Technologies' Flow Extruder Prototype.  //
// Developer: Cameron Palmer                                                                 //
// Last Modified: August 7, 2020                                                             //
///////////////////////////////////////////////////////////////////////////////////////////////

#define PROTOTYPE_VERSION "1.0"

// Included Libraries
#include <AccelStepper.h>
#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <Encoder.h>


#include <AverageThermocouple.h>      // Thermocouple libraries
#include <SmoothThermocouple.h>       //
#include <Thermocouple.h>             //
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
MAX6675_Thermocouple thermocouple(THERMO_SCK, THERMO_CS, THERMO_SO);

// LCD Object
LiquidCrystal lcd(RS, ENABLE, DB4, DB5, DB6, DB7);

// Encoder Object & Variables
Encoder enc(ENC_CLOCK, ENC_DATA);
long encOldPosition = -999;
unsigned long currentTime = 0;
//==================================

// Status display variables
short temperature = -99;
short extruderMotorStatus = 0;
short rollerMotorStatus = 0;
short levelMotorStatus = 0;
short winderMotorStatus = 0;

// Used for attaching to LiquidLines to make them focusable
void blankFunction() {
  return;
  
 }
 
// Menu creation
LiquidMenu menu(lcd);

LiquidLine welcomeLine0(0,0, "Flow Extruder");
LiquidLine welcomeLine1(0,1, "Prototype v", PROTOTYPE_VERSION); // Welcome screen
LiquidScreen welcomeScreen(welcomeLine0, welcomeLine1);

LiquidLine statusLine0(0,0, "Temp ", temperature, "C");
LiquidLine statusLine1(0,1,"Click for more");
LiquidScreen statusScreen(statusLine0, statusLine1);

LiquidLine optionsLine0(0,0, "Back");
LiquidLine optionsLine1(0,1, "Set temp");
LiquidLine optionsLine2(0,2, "Set ext spd");
LiquidLine optionsLine3(0,3, "Set rol spd");
LiquidLine optionsLine4(0,4, "Set lvl spd");
LiquidLine optionsLine5(0,5, "Set wind spd");
LiquidScreen optionsScreen(optionsLine0, optionsLine1, optionsLine2, optionsLine3); // LiquidScreen class constructor will only take 4 LiquidLines. The 5th and 6th are added below when the lcd is setup.


void setup() {
  
  // Start serial monitor for debugging purposes
  Serial.begin(9600);
  
  // Pinmodes
  pinMode(FAN, OUTPUT);
  pinMode(HEATER, OUTPUT);

  // More menu setup
  

  optionsLine0.attach_function(1, blankFunction);
  optionsLine1.attach_function(1, blankFunction);
  optionsLine2.attach_function(1, blankFunction);
  optionsLine3.attach_function(1, blankFunction);
  optionsLine4.attach_function(1, blankFunction);
  optionsLine5.attach_function(1, blankFunction);

  optionsScreen.set_displayLineCount(2);

  
  // Set up lcd, show welcome screen
  lcd.begin(16,2);
  optionsScreen.add_line(optionsLine4); // Adding the 5th and 6th lines to the options screen
  optionsScreen.add_line(optionsLine5);
  menu.add_screen(welcomeScreen);
  menu.add_screen(statusScreen);
  menu.add_screen(optionsScreen);
  menu.update();
  delay(3000);
  menu.next_screen();
  

 
 
}

void loop() {
  
  // Startup tests
  
    // Motor tests
    //m_extruder.runSpeed();
    //m_winder.runSpeed();
    //m_level.moveTo(100);
    //m_level.run();
  
  
    // Thermocouple tests
    //Serial.begin(9600);
    //Serial.println(thermocouple.readCelsius());
    //delay(1000);
    
    // Fan test
    
  
    // Heating tests
    //digitalWrite(4, HIGH);
    //delay(4000);
    //digitalWrite(4, LOW);

    
  // Reading encoder
  long newPosition = enc.read();
  if (newPosition != encOldPosition) {
    encOldPosition = newPosition; 
    menu.update();
    //Serial.println(newPosition);
  }

  // Reading thermocouple, storing value in short temperature if value needs to be updated, and updating menu to show updated temp
  if(temperature != short(thermocouple.readCelsius())){
      temperature = thermocouple.readCelsius();
      menu.update();
    }


  // Switching a screen when the encoder is clicked
  if(digitalRead(ENC_SWITCH) == LOW && (currentTime + 1000 <= millis())){

   if(menu.get_currentScreen() == &statusScreen){
      menu.change_screen(&optionsScreen);
    }
   
    currentTime = millis();
    
   }
  
  
  
  
  
}
