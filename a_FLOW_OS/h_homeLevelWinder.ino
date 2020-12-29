
//// void homeLevelWinder()

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

  while(m_level.currentPosition() != -1500){
    m_level.run();
    }
}
