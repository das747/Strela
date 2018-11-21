#include <Wire.h> //Include the Wire Library
#include <HTInfraredSeeker.h> //Include the IR Seeker Library

const int Seeker[] = {12, 5, 4,  3, 2, 0, 10, 9, 8, 7}; //преобразует зону мячика в направление 

//setup runs once at the beginning
void setup() {
  Serial.begin(9600); //Starts the Serial Monitor so we can read output
  Serial.println("Dir\tStrength\tdelay"); //Prints Dir & Strength at top
  InfraredSeeker::Initialize(); //initializes the IR sensor
}

//loop runs repeatedly
void loop() {   
  int timer = millis();
  //Read the AC (pulsed) information from the IR Sensor
  InfraredResult InfraredBall = InfraredSeeker::ReadAC();
  
  Serial.print(InfraredBall.Direction); //Print the Direction Number
  Serial.print("\t"); //Print a tab
  Serial.print(InfraredBall.Strength); //Print the Strength Number
  
  int n=millis()-timer;
  Serial.print("\t");
  Serial.print(n);
  Serial.println(); //Print a new line
}
