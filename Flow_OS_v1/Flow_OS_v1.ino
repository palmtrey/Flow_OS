///////////////////////////////////////////////////////////////////////////////////////////////
// Title: Flow OS v1                                                                         //
// Purpose: To create an operating system for Palmer Technologies' Flow Extruder Prototype.  //
// Developer: Cameron Palmer, palmertech3d@gmail.com                                         //
// Last Modified: September 11, 2020                                                          //
///////////////////////////////////////////////////////////////////////////////////////////////


#define PROTOTYPE_VERSION "1.0" // Prototype firmware version


// Included Libraries
#include <AccelStepper.h> // For motors
#include <LiquidCrystal.h> // For LCD
#include <LiquidMenu.h> // For menus on the LCD
#include <Encoder.h> // For rotary encoder
#include <PID_v1.h> // For PID control
#include <PID_AutoTune_v0.h>
#include <Thermocouple.h>          // Thermocouple libraries
#include <MAX6675_Thermocouple.h>  //
//==================================


// Pinout Constants
#define FAN 3
#define HEATER 4

#define THERMO_SCK 5
#define THERMO_CS 6   // Thermocouple constants
#define THERMO_SO 7

#define DB4 8 // LCD DB4
#define DB5 9 // LCD DB5
#define DB6 10 // LCD DB6
#define DB7 11 // LCD DB7     //Interface Constants
#define RS 12 // LCD RS
#define ENABLE 13 // LCD Enable
#define ENC_CLOCK 14 // Encoder clock
#define ENC_DATA 15 // Encoder data
#define ENC_SWITCH 16 // Encoder push button


#define M_EXTRUDER_STEP 46
#define M_EXTRUDER_DIR 47

#define M_ROLLER_STEP 48
#define M_ROLLER_DIR 49

#define M_LEVEL_STEP 50               //Motor Constants
#define M_LEVEL_DIR 51

#define M_WINDER_STEP 52
#define M_WINDER_DIR 53

#define WIND_LIM_SWITCH 17 // Winder limit switch: LOW means switch has been pressed
//==================================


// Function Declarations
void blankFunction(); // Used for attaching to LiquidLines to make them focusable, and thus scrollable
void returnPIDConstants(); // Displays constants for the PID calculated by the PID AutoTune library
void homeLevelWinder(); // Call to home the level winder; this function will delay all future action until the winder has been homed
//==================================


// Stepper objects
AccelStepper m_extruder(1, M_EXTRUDER_STEP, M_EXTRUDER_DIR);
AccelStepper m_roller(1, M_ROLLER_STEP, M_ROLLER_DIR);
AccelStepper m_level(1, M_LEVEL_STEP, M_LEVEL_DIR);
AccelStepper m_winder(1, M_WINDER_STEP, M_WINDER_DIR);
//==================================


// Thermocouple Object
Thermocouple* thermocouple = new MAX6675_Thermocouple(THERMO_SCK, THERMO_CS, THERMO_SO);
//==================================


// LCD Object
LiquidCrystal lcd(RS, ENABLE, DB4, DB5, DB6, DB7);
//==================================


// Encoder Object & Variables
Encoder enc(ENC_CLOCK, ENC_DATA);
long encOldPosition = -999;
long newPosition = 0;
unsigned long currentTime = 0;
//==================================


// Status display variables

//==================================


// PID object & variables
PID pid(&temperature, &output, &setTemp, Kp, Ki, Kd, DIRECT);
double temperature = -99;
double newTemp = 0;
double setTemp = 0;
double output;
double Kp = 2.16, Ki =0.08 , Kd = 15.28;
unsigned long pidTimekeeper = 0;
//==================================


// PID AutoTune setup
PID_ATune pidAuto(&temperature, &output);
bool stopPidAuto = false; // Set to true when the autotuner finishes
//==================================


// Motor variables
bool levelHomed = false; // Set to true when level winder has been homed

short extruderMotorStatus = 0; // The speed of the stepper motors in rpm
short rollerMotorStatus = 0;
short levelMotorStatus = 0;
short winderMotorStatus = 0;

//==================================


// Menu creation
LiquidMenu menu(lcd);

  // Create lines for the welcome screen and add them to it
  LiquidLine welcomeLine0(0,0, "Flow Extruder");
  LiquidLine welcomeLine1(0,1, "Prototype v", PROTOTYPE_VERSION); 
  LiquidScreen welcomeScreen(welcomeLine0, welcomeLine1); // Welcome screen
  
  // Create lines for the status screen and add them to it 
  LiquidLine statusLine0(0,0, temperature, "C");
  LiquidLine statusLine1(0,1,"Click for more");
  LiquidScreen statusScreen(statusLine0, statusLine1); // Status screen. Shows temp of heater and prompts the user to click for more options
  
  // Create lines for the options screen, initialize the options screen, but wait to add the lines to it until void setup()
  // This is because the LiquidScreen constructor can only take so many (4 maybe?) lines in its constructor
  LiquidLine optionsLine0(0,0, " Back"); // On screens with selectable elements, like the options screen, leave a space at the front of the string for the focus symbol
  LiquidLine optionsLine1(0,1, " Set temp ", setTemp, "C");
  LiquidLine optionsLine2(0,1, " Set ext spd");
  LiquidLine optionsLine3(0,1, " Set rol spd");
  LiquidLine optionsLine4(0,1, " Set lvl spd");
  LiquidLine optionsLine5(0,1, " Set wind spd"); // Options screen to change set temp, motor speeds, eventually (in prototype v2?) filament diameter
  LiquidScreen optionsScreen;
  
  LiquidLine tempLine0(0,0, "T: ", temperature, "C");
  LiquidLine tempLine1(0,1, "Set: <", setTemp, ">");
  LiquidScreen tempScreen(tempLine0, tempLine1);

//==================================


void setup() {
  
  // Start serial monitor for debugging purposes
  Serial.begin(9600);

  
  // Pinmodes
  pinMode(FAN, OUTPUT);
  pinMode(HEATER, OUTPUT);
  pinMode(WIND_LIM_SWITCH, INPUT);


  // Set up lcd
  lcd.begin(16,2);
  

  // Adding the lines to the options screen
  optionsScreen.add_line(optionsLine0);
  optionsScreen.add_line(optionsLine1);
  optionsScreen.add_line(optionsLine2);
  optionsScreen.add_line(optionsLine3);
  optionsScreen.add_line(optionsLine4);
  optionsScreen.add_line(optionsLine5);
  

  // Attach functions to options lines to make them scrollable
  optionsLine0.attach_function(1, blankFunction);
  optionsLine1.attach_function(1, blankFunction);
  optionsLine2.attach_function(1, blankFunction);
  optionsLine3.attach_function(1, blankFunction);
  optionsLine4.attach_function(1, blankFunction);
  optionsLine5.attach_function(1, blankFunction);
  

  // Set the number of lines the display has; this is necessary to allow scrolling
  optionsScreen.set_displayLineCount(2); 


  // Add the screens to the menu, set the focus position so the selecting arrow is on the left of menu items
  menu.add_screen(welcomeScreen); 
  menu.add_screen(statusScreen);
  menu.add_screen(optionsScreen);
  menu.add_screen(tempScreen);
  menu.set_focusPosition(Position::LEFT);
  

  // Begin initial device setup
  menu.update(); // Get the menu setup and ready to go
  homeLevelWinder(); // Home the level winder; this function will delay all future action until the winder has been homed
  menu.next_screen(); // Show the status screen
  

  // Start PID
  pid.SetMode(AUTOMATIC);


  // PID AutoTune
  pidAuto.SetControlType(1);


  // Stepper motor setup
  m_extruder.setMaxSpeed(1000); 
  m_extruder.setSpeed(200);

  m_roller.setMaxSpeed(1000);
  m_roller.setSpeed(200); // SAMPLE SPEED!!!!

  m_level
  
  m_winder.setMaxSpeed(1000);
  m_winder.setSpeed(50);

  

  
}

void loop() {

  

  m_level.run();
  // Limit switch test
  if(millis() >= currentTime + 500){
    Serial.println(digitalRead(WIND_LIM_SWITCH));
    currentTime = millis();  
  }

  // Motor test
  //m_extruder.runSpeed();
  //m_winder.runSpeed();
  
  if (millis() >= currentTime + 1000){
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

    // Encoder test output
    Serial.print("Old Pos: ");
    Serial.println(encOldPosition);
    Serial.print("New Pos: ");
    Serial.println(newPosition);
    Serial.println("------------");

    
    if(menu.get_currentScreen() == &optionsScreen && newPosition > encOldPosition){
     menu.switch_focus(false); // Focus backwards (up the screen) when encoder is turned counter-clockwise
    }
    
    if(menu.get_currentScreen() == &optionsScreen && newPosition < encOldPosition){
      menu.switch_focus(true); // Focus forwards (down the screen) when encoder is turned clockwise
    }
    
    if(menu.get_currentScreen() == &tempScreen){
      if (newPosition > encOldPosition){
        setTemp += (encOldPosition - newPosition); // Subtract when encoder is turned counter-clockwise
      }else{
        setTemp += (encOldPosition - newPosition); // Add when encoder is turned clockwise
      }
      
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
    } 
    // If current screen is the options screen, and the selected line is "Back"
    else if(menu.get_currentScreen() == &optionsScreen && menu.get_focusedLine() == 0){
      menu.change_screen(&statusScreen);
      menu.update();
    }

    // If current screen is the options screen, and the selected line is "Set temp"
    else if(menu.get_currentScreen() == &optionsScreen && menu.get_focusedLine() == 1){
      menu.change_screen(&tempScreen);
      menu.update();
    }

    else if(menu.get_currentScreen() == &tempScreen){
      menu.change_screen(&optionsScreen);
      menu.update();  
    }
   }

  // Reading thermocouple, storing value in short temperature if value needs to be updated, and updating menu if needed
  if (millis() >= pidTimekeeper + 500){
    newTemp = thermocouple->readCelsius();
    
    // Graphing temperature output vs.time
    /*
    Serial.println(newTemp);
    Serial.print(",");
    Serial.println(setTemp);
    */
    if(temperature != newTemp){
       temperature = newTemp;
        if (menu.get_currentScreen() == &statusScreen || menu.get_currentScreen() == &tempScreen){
          menu.update();
      }
    }
    pidTimekeeper = millis();
  }
  

   // Heater PID
   pid.Compute();
   analogWrite(HEATER, output);

   /*
   // Running PID autotuner for heater and checking if it's done
   if(!stopPidAuto){
    int PIDdone = pidAuto.Runtime();
    if(PIDdone == 1){
      returnPIDConstants();
      stopPidAuto = true;
     }
   }
   */ 
  
   
  
}
// Used for attaching to LiquidLines to make them focusable, and thus scrollable
void blankFunction() {
  return;
  
 }

void returnPIDConstants(){
  Serial.print("PID Kp = ");
  Serial.println(pidAuto.GetKp());
  Serial.print("PID Ki = ");
  Serial.println(pidAuto.GetKi());
  Serial.print("PID Kd = ");
  Serial.println(pidAuto.GetKd());
  return;
}

// Homes the level winder; this function will delay all future action until the winder has been homed
void homeLevelWinder(){
  
  // Get level winder ready to be homed
  m_level.setCurrentPosition(0); 
  m_level.moveTo(5000); // Get the level winder ready to move fast towards the limit switch
  m_level.setMaxSpeed(1000);
  m_level.setSpeed(500);
  m_level.setAcceleration(1000);
  
  while(!levelHomed){
    m_level.run(); // Move the level winder towards the limit switch

    if(digitalRead(WIND_LIM_SWITCH) == LOW){
      levelHomed = true; // Let us know that the level has been homed
      m_level.setCurrentPosition(0); // Set our starting pos to 0
      m_level.moveTo(-1500); // Move the level winder out from the limit switch
      
    }
    
  }
}
