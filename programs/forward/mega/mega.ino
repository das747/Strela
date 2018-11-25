#include <HTInfraredSeeker.h>

#define FIRST_US_PIN 27
#define FIRST_BUTTON_PIN 26
#define FIRST_MOTOR_IN 29


bool error[] = {0,0,0}, state[] = {0,0,0}; //для кнопок предыидущее значение и текущее состояние
int sp = 100, Xer = 2, dist = 100;                             //скорость, допуск гироскопа, ограничение для вратаря 
const int Seeker[] = {0 , 6, 5, 4, 2, 0, 0, 8, 7, 6};
const int IRer[] = {0, 2, 3,  3, 4, 5,  6, 8, 4,  7, 7, 8,  8, 9, 10, 12}; //преобразует зону мячика в направление движения
const int IRm[] = {8, 9, 10, 0, 2, 3, 4};
long X;                                    //для гироскопа

//=======================================================_____SETUP_____=========================================================
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); //связь с наной
  Serial.println("setup");
 
  //InfraredSeeker::Initialize(); //инициализация леговского датчика
}


void loop() {
  Serial.println("loop");
  CheckButtons();
  int a;
  int US2;
  int US0;
  //==========================================_____DEBUG_____===============================================
  if(state[2]){ //дебаг
    Serial.println("DEBUG");
    M1r(0);
    M2r(0);
    M3r(0);
    GyroUpdate();
    a = SeekerZ();
    //IRz();
    US0 = getUS(0);
    US2 = getUS(2);
    if(getUS(1) < 8 and a == 0) { //если мяч перед роботом, то
      Serial.print("\tattack:");
      if((abs(US2 - US0) >= 40)){   //если робот смещён вбок то выравниваемся
        if(US2 < US0) {
          Serial.print("left");
        }else{
          Serial.print("right");
        }
      }else{
        Serial.print("centre");
      }
    }
    Serial.print("\t");
    Serial.print(pulseIn(50, LOW, 2000));
    Serial.print("\t");
    Serial.println(LINE(a));
    
    
  }

  
  //==========================================_____START_____===============================================
  if(state[1] or state[0]){   //рабочий режим: первая кнопка - нападающий; вторая кнопка - вратарь
    if(state[0]) Serial.println("FORWARD");
    else Serial.println("GOALKEEPER");
    a = SeekerZ();
    US2 = getUS(2);
    US0 = getUS(0);

    //if(((US2<50) or (US2<50))and((US2+US3)>165)) sp=70; //если на одном из датчиков <50 и сумма датчиков >165, то едем медленнее
    //else sp=100;
    if(a == 0 and getUS(1) < 8) { //если мяч перед роботом, то
      Serial.print("\tattack:");
      if((abs(US2 - US0) >= 40)){   //если робот смещён вбок то выравниваемся
        if(US2 < US0) {
          a = 10;
          Serial.print("left");
        }else{
          a = 2;
          Serial.print("right");
        }
      }else{
        Serial.print("centre");
      }
    }    
    GoToZone(LINE(a));
    Serial.println();
  }
}



//==========================================_____FUNCTIONS_____===============================================
  

bool GoToZone(int zone) {             //едет в указанную зону                                                                      ОК
  int b;
  GyroUpdate();
  b = (X > Xer or X < (-Xer)) * X * X * X / 200;
  //b = X * 1 * (X > Xer or X < (-Xer));
  for(int i = 0; i<4; i++){
    runMotor(i, constrain(sp * cos(PI * (angle - M[i]) / 180), -255, 255));
  }
  Serial.print("\tMoving to ");
  Serial.print(zone);
  Serial.print("(");
  Serial.print(b);
  Serial.print(")");
  return true;
}

void runMotor(int n, int sp){
  bool dir = sp < 0;
  digitalWrite(in1 + n * 4 + 2 * (n > 1), !dir and sp != 0);
  digitalWrite(in2 + n * 4 - 2 * (n > 1), dir and sp != 0);
  analogWrite(en - n, abs(sp));
}


bool GyroUupdate() //обновление значения гироскопа
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

//резерв(от -255 до 255);
/*bool GiroUpdate() 
{
  Serial1.write(1);
  while (!Serial1.available()) {
    Serial.println("Updating giro...");
  }
  X = Serial1.read();
  if (digitalRead(23)) X = X * -1;
  Serial.print("GiroValue:                  ");
  Serial.println(X);
}*/


int IRz() {               //определение зоны                                                                                       ПРОВЕРИТЬ
  int n = (pulseIn((39),LOW,2000));
  int s = 0;
  for(int i = 1; i < 7; i++){
    if(pulseIn((39 + i * 2), LOW, 2000) > n){
      s = i;
      n = (pulseIn((39 + i * 2), LOW, 2000));
    }
  }
  int r;
  if(n == 0) r = 12;
  else  r = (IRm[s]);
  Serial.print("\tIRzone:");
  Serial.print(r);
  return (r);
}

int LINE(int zone) {      //проверяет не едем ли мы на линию                                                                        ПЕРЕДЕЛАТЬ
  if((zone > 9 or zone < 3) and digitalRead(44)) zone = (zone + 6) % 12;
  //if((zone < 6) and digitalRead(38)) zone = (zone + 6) % 12;
  if((zone < 9 and zone > 3) and (digitalRead(46) or digitalRead(42))) zone = (zone + 6) % 12;
  //if((zone > 6) and digitalRead(40)) zone = (zone + 6) % 12;
  return(zone);
}

int getUS(int US){       // берёт значение с ультразвукового датчика                                                                ПРОВЕРИТЬ
  int value;
  int trig = FIRST_US_PIN + US * 2;
  int echo = trig + 6;
  if(trig == 27) trig = 25;
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

  
int SeekerZ() { //зона с мячом                                                                                                        ПРОВЕРИТЬ
  InfraredResult InfraredBall = InfraredSeeker::ReadAC();                             
  int dir = InfraredBall.Direction;
  int strength = InfraredBall.Strength;
  Serial.print("\tIR direction:");
  Serial.print(dir); //Print the Direction Number
  Serial.print("(");
  Serial.print(strength);
  Serial.print(")");
  if((pulseIn(50, LOW, 2000)) > 150){
    switch(dir){
      case 7:
        dir=5;
        break;
      case 8:
        dir=5;
        break;
      case 9:
        dir=5;
        break; 
      case 1:
        dir=7;
        break;
      case 0:
        dir=3;
        break;
    }
  }else{
    dir = Seeker[dir]; //преобразуем зону в направление движения
    if(state[1]){
      if(strength<dist){
        dir = 12; //в режиме вратаря игнорирум мяч, если он далеко
      }
    }
  }
  
  Serial.print("-->");
  Serial.print(dir); 
  return dir;
}

bool CheckButtons(){  //управление режимами при помощи кнопок                                                                   ПРОВЕРИТЬ
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
          
