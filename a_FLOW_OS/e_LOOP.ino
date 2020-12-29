
//// LOOP FUNCTION

void loop() {

  if(millis() >= currentTime + 500){
    currentTime = millis();  
  }
  


  // Reading encoder turns
  currentStateCLK = digitalRead(ENC_CLK);

  if (currentStateCLK != lastStateCLK && currentStateCLK == 1){
    lastEncPos = encPos;
    if (digitalRead(ENC_DATA) != currentStateCLK){
        encPos--;
      }else{
        encPos++;
      }

      
  }

  
  

  
  if ((currentStateCLK != lastStateCLK) && (menu.get_currentScreen() != &statusScreen)) {
    lastStateCLK = currentStateCLK;
    // Encoder test output
    
    Serial.print("Old Pos: ");
    Serial.println(lastEncPos);
    Serial.print("New Pos: ");
    Serial.println(encPos);
    Serial.println("------------");
  
    
    if(menu.get_currentScreen() == &optionsScreen && encPos > lastEncPos){
     menu.switch_focus(false); // Focus backwards (up the screen) when encoder is turned counter-clockwise
    }
    
    if(menu.get_currentScreen() == &optionsScreen && encPos < lastEncPos){
      menu.switch_focus(true); // Focus forwards (down the screen) when encoder is turned clockwise
    }
    
    if(menu.get_currentScreen() == &tempScreen){
      if (encPos > lastEncPos){
        setTemp += (lastEncPos - encPos); // Subtract when encoder is turned counter-clockwise
      }else{
        setTemp += (lastEncPos - encPos); // Add when encoder is turned clockwise
      }
      
    }
    menu.update();
  }


  // Reading encoder button
  if(digitalRead(ENC_SWITCH) == LOW && (currentTime + 500 <= millis())){
    
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
