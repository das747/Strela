#include <L3G4200D.h>
#include <AFMotor.h>

AF_DCMotor M1(1);                                     //создание объектов мотор
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);
L3G4200D gyro;  //создание гироскопа

float X = 0;                    //переменная для значения с гироскопа
int timeStep=50, sp=200, angle=0;  
int M[]={315,225,135,45};



void setup() {
  Serial.begin(9600);
  M1.setSpeed(sp);    //включаем моторы
  M1.run(RELEASE);
  M2.setSpeed(sp);
  M2.run(RELEASE);
  M3.setSpeed(sp);
  M3.run(RELEASE);
  M4.setSpeed(sp);
  M4.run(RELEASE);
  while (!gyro.begin(L3G4200D_SCALE_2000DPS, L3G4200D_DATARATE_400HZ_50)) { // включаем  и калибруем гироскоп
    Serial.println("wait");
    delay(500);
  }
  gyro.calibrate(100);
}


void loop(){
  unsigned long timer=millis();
  GyroUpdate();
  int b=X;
  for(int i = 0; i<4; i++){
    runMotor(i, constrain(sp * cos(PI * (angle - M[i]) / 180), -255, 255));
  }
  Serial.println(X);
  delay(timeStep-(millis()-timer));
//  
}





void GyroUpdate(){
  Vector norm = gyro.readNormalize();
  bool a = (abs(norm.ZAxis * timeStep / 1000) > 0.05);
  X += norm.ZAxis * timeStep / 1000 * a;
}

void runMotor(int num, int speed){
  switch (num) {
    case 0:
        M1.setSpeed(abs(speed));
        if (speed == 0) M1.run(RELEASE); //絘
        else {
          if (speed < 0) M1.run(BACKWARD);
          else M1.run(FORWARD);
        }
    case 1:       
        M2.setSpeed(abs(speed));
        if (speed == 0) M2.run(RELEASE); //絘
        else {
          if (speed < 0) M2.run(BACKWARD);
          else M2.run(FORWARD);
        }
    case 2:
        M3.setSpeed(abs(speed));
        if (speed == 0) M3.run(RELEASE); //絘
        else {
          if (speed < 0) M3.run(BACKWARD);
          else M3.run(FORWARD);
        }
    case 3:
        M4.setSpeed(abs(speed));
        if (speed == 0)M4.run(RELEASE); //絘
        else {
          if (speed < 0)M4.run(BACKWARD);
          else M4.run(FORWARD);
        }
  }
}

