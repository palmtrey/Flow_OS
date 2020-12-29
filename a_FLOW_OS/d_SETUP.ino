
//// SETUP FUNCTION

void setup() {
  
  // Start serial monitor for debugging purposes
  Serial.begin(9600);

  
  // General pinmodes
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


  // Add the screens to the menu
  menu.add_screen(welcomeScreen); 
  menu.add_screen(statusScreen);
  menu.add_screen(optionsScreen);
  menu.add_screen(tempScreen);

  // Set the focus position so the selecting arrow is on the left of menu items
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

  
  m_winder.setMaxSpeed(1000);
  m_winder.setSpeed(50);

  
  // Encoder setup
  pinMode(ENC_CLK, INPUT);
  pinMode(ENC_DATA, INPUT);
  pinMode(ENC_SWITCH, INPUT);

  lastStateCLK = digitalRead(ENC_CLK);

  
}
