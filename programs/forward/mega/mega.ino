#include <HTInfraredSeeker.h>

#define FIRST_US_PIN 47
#define FIRST_BUTTON_PIN 48
#define FIRST_MOTOR_IN 29
#define FIRST_ENABLE_PIN 12


bool error[] = {0,0,0}, state[] = {0,0,0}; //для кнопок предыидущее значение и текущее состояние
const int sp_ref = 200, X_er = 2;                             //основная скорость, допуск гироскопа
const int IRpins[] = {38, 36, 34,  32, 30, 28,  26, 24, 46,  44, 42, 23,  25, 27, 40};
const int M[] = {325, 35, 325, 35};
long X;                                    //для гироскопа

//=======================================================_____SETUP_____=========================================================
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); //связь с наной
  Serial.println("setup");
  for(int i=29; i <= 43; i+=2){
    pinMode(i, OUTPUT);
  }
    for(int i=12; i >=9; i--){
    pinMode(i, OUTPUT);
  }
}


void loop() {
  Serial.println("loop");
  CheckButtons();
  int dir, right, left, sp;
  //==========================================_____DEBUG_____===============================================
  if(state[2]){ //дебаг
    Serial.println("DEBUG");
    GyroUpdate();
    GoToZone(0, 0);
    dir = IRz();
    sp = sp_ref * (dir == 747);
    left = getUS(0);
    right = getUS(1);
    if(getUS(7) < 8 and dir == 0) { //если мяч перед роботом, то
      Serial.print("\tattack:");
      if((abs(left - right) >= 40)){   //если робот смещён вбок то выравниваемся
        if(right < left) {
          Serial.print("left");
        }else{
          Serial.print("right");
        }
      }else{
        Serial.print("centre");
      }
    }
    Serial.print("\tsp=");
    Serial.print(sp);
    Serial.print("\t");
    Serial.println(LINE(dir));
    
    
  }

  
  //==========================================_____START_____===============================================
  if(state[1]){   //рабочий режим: первая кнопка - нападающий; вторая кнопка - вратарь
    if(state[0]) Serial.println("FORWARD");
    else Serial.println("GOALKEEPER");
    dir  = IRz;
    sp = sp_ref * (dir == 747);
    right = getUS(1);
    left = getUS(0);
    //if(((left < 50) or (right < 50)) and ((left + right) > 165)) sp -= 50; //если на одном из датчиков <50 и сумма датчиков >165, то едем медленнее
    /*if(dir == 0 and getUS(7) < 8) { //если мяч перед роботом, то
      Serial.print("\tattack:");
      if((abs(left - right) >= 40)){   //если робот смещён вбок то выравниваемся
        if(right < left) {
          dir = 20
          Serial.print("left");
        }else{
          dir = 340;
          Serial.print("right");
        }
      }else{
        Serial.print("centre");
      }
    }    */
    Serial.print("\tsp=");
    Serial.print(sp);
    GoToZone(0, sp);
    Serial.println();
  }

  if(state[0]){
    for(int i=0; i < 4; i++){
      Serial.println(i);
      fullStop(i);
      runMotor(i, 150);
      delay(500);
      runMotor(i, -150);
      delay(500);
    }
  }
}



//==========================================_____FUNCTIONS_____===============================================
  

bool GoToZone(int angle, int m_sp) {             //едет в указанную зону                                                                      ОК
  GyroUpdate();
  int g_er = (X > X_er or X < (-X_er)) * X * X * X / 200;
  //g_er = X * 1 * (X > Xer or X < (-Xer));
  for(int i = 0; i<4; i++){
    runMotor(i, constrain(m_sp * cos(PI * (angle - M[i]) / 180) + g_er, -255, 255));
  }
  Serial.print("\tMoving to ");
  Serial.print(angle);
  Serial.print("(");
  Serial.print(g_er);
  Serial.print(")");
  return true;
}

void runMotor(int n, int sp){ //                                                                                                   ОК
  Serial.print("\tM");
  Serial.print(n + 1);
  Serial.print(':');
  Serial.print(sp); 
  bool dir = sp < 0 and abs(sp);
  digitalWrite(FIRST_MOTOR_IN + n * 4 + 2 * (n > 1), !dir and abs(sp));
  digitalWrite(FIRST_MOTOR_IN + 2 + n * 4 - 2 * (n > 1), dir and abs(sp));
  analogWrite(FIRST_ENABLE_PIN - n, abs(sp));
} 

bool fullStop(int m){
  if(m != 0) runMotor(0, 0);
  if(m != 1) runMotor(1, 0);
  if(m != 2) runMotor(2, 0);
  if(m != 3) runMotor(3, 0);
  
  return true;
}


bool GyroUpdate() //обновление значения гироскопа                                                                                 ОК                                                                                                                             
{
  Serial1.write(1);
  while (!Serial1.available()) {
  }
  X = Serial1.read();
  if (X > 128) X = X - 256;
  Serial.print("\tGyroValue:");
  Serial.print(X);
  return true;
}

int IRz() {               //определение зоны                                                                                       ПРОВЕРИТЬ
  int max_val = 0;
  int max_sens = -1;
  for(int i = 0; i < 15; i++){
    int val = pulseIn(IRpins[i], LOW, 2000);
    if(val > max_val){
      max_sens = i;
      max_val = val;
    }
  }
  int angle = (180 - max_sens * 24) * bool(max_sens) + 747 * !bool(max_sens);
  Serial.print("\tIRzone:");
  Serial.print(angle);
  return angle;
}

int LINE(int zone) {      //проверяет не едем ли мы на линию                                                                        ПЕРЕДЕЛАТЬ
  if((zone > 9 or zone < 3) and digitalRead(44)) zone = (zone + 6) % 12;
  if((zone < 6) and digitalRead(38)) zone = (zone + 6) % 12;
  if((zone < 9 and zone > 3) and (digitalRead(46) or digitalRead(42))) zone = (zone + 6) % 12;
  if((zone > 6) and digitalRead(40)) zone = (zone + 6) % 12;
  return zone;
}

int getUS(int US){       // берёт значение с ультразвукового датчика                                                                ПРОВЕРИТЬ
  int value;
  int trig = FIRST_US_PIN + US * 6;
  int echo = FIRST_US_PIN + 2 + US * 2;
  digitalWrite(trig, LOW); //НАЧАЛО ПОЛУЧЕНИЯ ДАННЫХ С US ДАТЧИКА
  delayMicroseconds(5);
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trig, LOW); //КОНЕЦ ПОЛУЧЕНИЯ ДАННЫХ С US ДАТЧИКА
  value = pulseIn(echo, HIGH, 20000); //НАЧАЛО ОБРАБОТКИ ПОКАЗАНИЙ US
  value = (value/2) / 29.1;        //КОНЕЦ ОБРАБОТКИ ПОКАЗАНИЙ US
  if(value <= 0) value = 0;
  Serial.print("\tUS");
  Serial.print(US);
  Serial.print(":");
  Serial.print(value);
  return value;
}

bool CheckButtons(){  //управление режимами при помощи кнопок                                                                   OK
  bool D;
  for(int i = 0; i < 3; i++){
    D = digitalRead(FIRST_BUTTON_PIN + i * 2);
    if(D != error[i]) {
      error[i] = D;
      if(D) {
        state[i] =! state[i];
        state[(i + 1) % 3] = 0;
        state[(i + 2) % 3] = 0;
      } 
    } 
  }
  return true;
}
          
