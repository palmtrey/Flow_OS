
//// MENU CREATION

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


// Create lines for the temp screen and add them to it

LiquidLine tempLine0(0,0, "T: ", temperature, "C");
LiquidLine tempLine1(0,1, "Set: <", setTemp, ">");
LiquidScreen tempScreen(tempLine0, tempLine1);
