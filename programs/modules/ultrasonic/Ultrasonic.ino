#define FIRST_US_PIN 4
void setup() {
  Serial.begin(9600);
  pinMode(FIRST_US_PIN,OUTPUT);//передний датчик - 1; эхо=35
  pinMode(25,OUTPUT);//правый датчик - 0; эхо=33
  pinMode(31,OUTPUT);//левый датчик - 2; эхо=37
  
}

void loop() {
Serial.println(getUS(0));
}


int getUS(int US){
  int value;
  int trig = FIRST_US_PIN + US * 6;
  int echo = FIRST_US_PIN - 2 + US * 2;
  digitalWrite(trig, LOW); //НАЧАЛО ПОЛУЧЕНИЯ ДАННЫХ С US ДАТЧИКА
  delayMicroseconds(5);
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trig, LOW); //КОНЕЦ ПОЛУЧЕНИЯ ДАННЫХ С US ДАТЧИКА
  value = pulseIn(echo, HIGH); //НАЧАЛО ОБРАБОТКИ ПОКАЗАНИЙ US
  value = (value/2) / 29.1;        //КОНЕЦ ОБРАБОТКИ ПОКАЗАНИЙ US
  if(value<=0) value=0;
  return value;
}
