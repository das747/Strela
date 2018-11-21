int X;

void setup() {
Serial.begin(9600);
Serial1.begin(9600);

}

void loop() {
  Serial1.write(1);
  while(!Serial1.available()){}
  X = Serial1.read();
  if(digitalRead(46)) X=X*-1;
  Serial.println(X);

}
