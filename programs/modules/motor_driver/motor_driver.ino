void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

void run_motor(int n, int sp){
  bool dir = sp < 0;
  digitalWrite(in1 + n, dir and sp != 0);
  digitalWrite(in2 + n, !dir and sp != 0);
  analogWrite(en + n, abs(sp));
}

