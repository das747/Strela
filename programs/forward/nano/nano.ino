#include <L3G4200D.h>

L3G4200D gyro;              //создание гироскопа

float X = 0;                    //переменная для значения с гироскопа
int timeStep=50;  
void setup() {
  Serial.begin(9600);
  //disp.init(D4056A);  //Инициализация модуля
  //disp.set(BRIGHT_TYPICAL); //яркость * BRIGHT_TYPICAL = 2 Средний * BRIGHT_DARKEST = 0 Тёмный  * BRIGHTEST = 7 Яркий
  while (!gyro.begin(L3G4200D_SCALE_2000DPS, L3G4200D_DATARATE_400HZ_50)) { // включаем  и калибруем гироскоп
    Serial.println("wait");
    delay(500);
  }
  gyro.calibrate(100);
}
void loop(){
  unsigned long timer = millis();
  Vector norm = gyro.readNormalize();
  bool a = (abs(norm.ZAxis * timeStep / 1000) > 0.05);
  X += norm.ZAxis * timeStep / 1000 * a;
  //digitalWrite(8,(X<0)); 
  if(Serial.available()){
    Serial.read();
    Serial.write(constrain(int(X), -127, 127));     // передача в диапазоне (-128/128)
    //Serial.write(constrain(int(abs(X)),0,255));    резерв с диапазоном (-256/256)
  }
  delay(timeStep - (millis() - timer));
  
}


