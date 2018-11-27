int in1 = 29;
int in2 = 31; 
int en = 12;
bool i = 0;
void setup() {
  for(int i=12; i >=9; i--){
    pinMode(i, OUTPUT);
  }
  for(int i=29; i <= 43; i+=2){
    pinMode(i, OUTPUT);
  }

}
void loop() {
run_motor(0, 150 * (1 - i * 2));
i = !i;
delay(1000);
}

void run_motor(int n, int sp){
  bool dir = sp < 0;
  digitalWrite(in1 + n * 4 + 2 * (n > 1), !dir and sp != 0);
  digitalWrite(in2 + n * 4 - 2 * (n > 1), dir and sp != 0);
  analogWrite(en - n, abs(sp));
}

