
int kicker = 4;


void setup() {
  Serial.begin(9600);
  pinMode(kicker, OUTPUT);
}

void loop() {
  Serial.println(1);
  digitalWrite(kicker, HIGH);
  delay(1000);
  Serial.println(0);
  digitalWrite(kicker, LOW);
  delay(1000);
}
