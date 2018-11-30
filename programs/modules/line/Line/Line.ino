 #define FIRST_LINE_SENSOR A3

void setup() {
Serial.begin(9600);
}

void loop() {
for(int i=0;i<4;i++){
  Serial.print("\tD");
  Serial.print(i+FIRST_LINE_SENSOR);
  Serial.print(":");
  Serial.print(analogRead(FIRST_LINE_SENSOR+i));

}

Serial.println();
delay(100);
}
