#include <L3G4200D.h>
#include <AFMotor.h>

AF_DCMotor M1(1);                                     //создание объектов мотор
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);
L3G4200D gyro;  //создание гироскопа



float X = 0;                    //переменная для значения с гироскопа
int timeStep=50;  
void setup() {
  Serial.begin(9600);
  while (!gyro.begin(L3G4200D_SCALE_2000DPS, L3G4200D_DATARATE_400HZ_50)) { // включаем  и калибруем гироскоп
    Serial.println("wait");
    delay(500);
  }
  gyro.calibrate(100);
}
void loop(){
  unsigned long timer=millis();
  Vector norm = gyro.readNormalize();
  bool a = (abs(norm.ZAxis * timeStep / 1000) > 0.05);
  X += norm.ZAxis * timeStep / 1000 * a;
  Serial.println(X);
  delay(timeStep-(millis()-timer));
//  
}


