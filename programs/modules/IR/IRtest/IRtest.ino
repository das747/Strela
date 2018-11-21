void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(pulseIn(4, LOW, 2000));
  
}
