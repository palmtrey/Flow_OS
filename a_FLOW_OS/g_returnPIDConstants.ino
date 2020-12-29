
//// void returnPIDConstants()

void returnPIDConstants(){
  Serial.print("PID Kp = ");
  Serial.println(pidAuto.GetKp());
  Serial.print("PID Ki = ");
  Serial.println(pidAuto.GetKi());
  Serial.print("PID Kd = ");
  Serial.println(pidAuto.GetKd());
  return;
}
