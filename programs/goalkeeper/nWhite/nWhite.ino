
#include <L3G4200D.h>
#include <TM1637.h>
#include <HTInfraredSeeker.h>


#define CLK 11    
#define DIO 10    

L3G4200D gyro;              //создание гироскопа
TM1637 disp(CLK,DIO);
float X = 0;                //переменная для значения с гироскопа
int timeStep=25;  
void setup() {
  Serial.begin(9600);
  //disp.init(D4056A);      //Инициализация модуля
  //disp.set(BRIGHT_TYPICAL); //яркость    * BRIGHT_TYPICAL = 2 Средний * BRIGHT_DARKEST = 0 Тёмный  * BRIGHTEST = 7      Яркий
  while (!gyro.begin(L3G4200D_SCALE_2000DPS, L3G4200D_DATARATE_400HZ_50)) { // включаем  и калибруем гироскоп
    Serial.println("wait");
    delay(500);
  }
  gyro.calibrate(100);          //калибровка гироскопа
  InfraredSeeker::Initialize(); //инициализация леговского датчика
}
void loop(){
  unsigned long timer=millis(); //получаем значение с гироскопа
  Vector norm = gyro.readNormalize();
  bool a = (abs(norm.ZAxis * timeStep / 1000) > 0.05);
  X += norm.ZAxis * timeStep / 1000 * a;

  int dir, strength;
  InfraredResult InfraredBall = InfraredSeeker::ReadAC(); //обновление значений
  dir=InfraredBall.Direction; //направление на мяч
  strength=InfraredBall.Strength;

  //digitalWrite(8,(X<0)); //альт
  if(Serial.available()){   //отправка значений
    int recieve = Serial.read();
    switch (recieve){
      case 1:
        Serial.write(constrain(int(X),-127,127));
        //Serial.write(constrain(int(abs(X)),0,255));   //альт  
        break;       
      case 2:
        Serial.write(dir);
        break;
      case 3:
        Serial.write(strength);
        break;
    }
     
    
    
  }
  delay(timeStep-(millis()-timer));
  
}


