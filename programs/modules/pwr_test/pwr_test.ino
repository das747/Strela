int in1 = 29;
int in2 = 31; 
int en = 12;
void setup() {
  for(int i=29; i < 43; i+=2){
    pinMode(i, OUTPUT);
  }
  Serial.begin(9600);
}
void loop() {
  for(int i = 0; i < 256; i++){
    run_motor(0, i); 
    delay(25);
  }
  for(int i = 256; i > 0; i--){
    run_motor(0, i); 
    delay(25);
  }
}

void run_motor(int n, int sp){
  bool dir = sp < 0;
  digitalWrite(in1 + n * 4 + 2 * (n > 1), !dir and sp != 0);
  digitalWrite(in2 + n * 4 - 2 * (n > 1), dir and sp != 0);
  analogWrite(en - n, abs(sp));
}

