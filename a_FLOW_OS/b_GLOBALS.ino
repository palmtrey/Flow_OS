
//// GLOBAL OBJECTS & VARIABLES


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


// PID object & variables
double temperature = -99; // Used to display the temperature on the LCD
double newTemp = 0; // A temporary temperature storage place used when reading the thermocouple
double setTemp = 0;
double output;
double Kp = 2.16, Ki =0.08 , Kd = 15.28;
unsigned long pidTimekeeper = 0;
PID pid(&temperature, &output, &setTemp, Kp, Ki, Kd, DIRECT);
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

// Encoder Object & Variables
int encPos = 0;
int lastEncPos = 0;
int currentStateCLK, lastStateCLK;
int currentTime = millis();
//==================================
