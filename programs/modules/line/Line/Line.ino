 #define FIRST_LINE_SENSOR 36

void setup() {
Serial.begin(9600);
}

void loop() {
for(int i=0;i<5;i++){
  Serial.print("\tD");
  Serial.print(i*2+FIRST_LINE_SENSOR);
  Serial.print(":");
  Serial.print(digitalRead(FIRST_LINE_SENSOR+i*2));

}

Serial.println();
delay(100);
}
