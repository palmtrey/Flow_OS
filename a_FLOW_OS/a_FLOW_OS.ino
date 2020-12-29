///////////////////////////////////////////////////////////////////////////////////////////////
// Title: Flow OS v1                                                                         //
// Purpose: To create an operating system for Palmer Technologies' Flow Extruder Prototype.  //
// Developer: Cameron Palmer, palmertech3d@gmail.com                                         //
// Last Modified: September 22, 2020                                                          //
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
#define ENC_CLK 14 // Encoder clock
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

#define WIND_LIM_SWITCH 17 // Winder limit switch: Active LOW

//==================================



// Function Declarations

void blankFunction(); // Used for attaching to LiquidLines to make them focusable, and thus scrollable
void returnPIDConstants(); // Displays constants for the PID calculated by the PID AutoTune library
void homeLevelWinder(); // Call to home the level winder; this function will delay all future action until the winder has been homed

//==================================
