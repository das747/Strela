#include <HTInfraredSeeker.h>
#include <AFMotor.h>

#define FIRST_US_PIN 27
#define FIRST_BUTTON 26



AF_DCMotor M1(4);                                     //создание объектов мотор
AF_DCMotor M2(2);
AF_DCMotor M3(3);
bool error[]={0,0,0}, state[]={0,0,0}, START,DEBUG,AAA; //для кнопок предыидущее значение и текущее состояние
int sp = 100      , dif = 1, dist = 60, RECIEVE[8], BUF[8];                                //скорость,точность зоны мячика, зона с мячиком
const int Seeker[] = {12, 6, 5, 4, 2, 0, 0, 8, 7, 6};
const int IRer[] = {0, 2, 3,  3, 4, 5,  6, 8, 4,  7, 7, 8,  8, 9, 10, 12}; //преобразует зону мячика в направление движения
const int IRm[]={8, 9, 10, 0, 2, 3, 4};
const int A[] = {  2,  1,  0, -1, -2, -2, -2, -1,  0,  1,  2,  2, 0}; // массивы для движения
const int B[] = { -2, -2, -2, -1,  0,  1,  2,  2,  2,  1,  0, -1, 0};
const int C[] = {  0,  1,  2,  2,  2,  1,  0, -1, -2, -2, -2, -1, 0};
int X;                                    //для гироскопа

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); //связь с наной
  Serial.println("setup");
  M1.setSpeed(sp);    //включаем моторы
  M1.run(RELEASE);
  M2.setSpeed(sp);
  M2.run(RELEASE);
  M3.setSpeed(sp);
  M3.run(RELEASE);
  pinMode(29,OUTPUT);//передний датчик - 1; эхо=35
  pinMode(27,OUTPUT);//правый датчик - 0; эхо=33
  pinMode(31,OUTPUT);//левый датчик - 2; эхо=37
  InfraredSeeker::Initialize(); //инициализация леговского датчика
}

void loop() {
  Serial.println("loop");
  CheckButtons();
  
  if(AAA){
  getUS(1);
  getUS(2);
  getUS(0);
  Serial.println();
  }

  if(DEBUG){
    GyroUpdate();
    IRz();
    Serial.print("                                      ");
    Serial.println(pulseIn(50,LOW,2000));
    
    
  }

  if(START){
    int a = IRz();
    GoToZone(LINE(a),10,2);
  }else{
    M1r(0);
    M2r(0);
    M3r(0);
  }

  /*if(digitalRead(31)==1) GO = 1;   //пока не нажата кнопка не включать двичение
    int Zone=IRer[IRz()];
    if(GO){
    int US2=getUS(2);
    int US3=getUS(3);
    //if(((US2<50) or (US2<50))and((US2+US3)>165)) sp=70; //если на одном из датчиков <50 и сумма датчиков >165, то едем медленнее
    //else sp=100;
    if((Zone==0)and(getUS(1)<10)) { //если мяч перед роботом, то
      Serial.println("ball");
      if(!(abs(US2-US3)>=40)){   //если робот смещён вбок то выравниваемся
        if(US2>US3) Zone=10;
        else Zone=2;
      }
    }
    GoToZone(Zone); //если мяч не у робота или мяч у робота и смещения нет, то едем на мяч
    Serial.println(Zone);
    }*/
}

bool GoToZone(int zone, float n,float k) {                 //едет в указанную зону
  int b;
  GyroUpdate();
  if (X < 0) b = (abs(X) > 1) * (X-n)*k; //вычисляем поправку от гироскопа
  else  b = (abs(X) > 1)*(X+n)*k;
  M1r(constrain((sp * A[zone] + b), -255, 255));
  M2r(constrain((sp * B[zone] + b), -255, 255));
  M3r(constrain((sp * C[zone] + b), -255, 255));
  Serial.print("                       Moving to ");
  Serial.println(zone);
  return true;
}

bool M1r(int b) {               //УПРОЩЁННОЕ УПРАВЛЕНИЕ МОТОРАМИ: ОТРИЦАТЕЛЬНАЯ МОЩНОСТЬ - НАЗАД, 0 - СТОП
  M1.setSpeed(abs(b));
  if (b == 0)M1.run(RELEASE); //絘
  else {
    if (b < 0)M1.run(BACKWARD);
    else M1.run(FORWARD);
  }
  return true;
}
bool M2r(int b) {               //УПРОЩЁННОЕ УПРАВЛЕНИЕ МОТОРАМИ: ОТРИЦАТЕЛЬНАЯ МОЩНОСТЬ - НАЗАД, 0 - СТОП
  M2.setSpeed(abs(b));
  if (b == 0)M2.run(RELEASE); //絘
  else {
    if (b < 0)M2.run(BACKWARD);
    else M2.run(FORWARD);
  }
  return true;
}

bool M3r(int b) {               //УПРОЩЁННОЕ УПРАВЛЕНИЕ МОТОРАМИ: ОТРИЦАТЕЛЬНАЯ МОЩНОСТЬ - НАЗАД, 0 - СТОП
  M3.setSpeed(abs(b));
  if (b == 0)M3.run(RELEASE); //絘
  else {
    if (b < 0)M3.run(BACKWARD);
    else M3.run(FORWARD);
  }
  return true;
}

bool GyroUpdate() //обновление значения гироскопа
{
  Serial1.write(1);
  while (!Serial1.available()) {
    Serial.println("Updating giro...");
  }
  X = Serial1.read();
  if (X>128) X=X-256;
  Serial.print("GiroValue:                  ");
  Serial.println(X);
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


int IRz() {               //определение зоны
  int n=(pulseIn((39),LOW,2000));
  int s=0;
  for(int i=1;i<7;i++){
    if(pulseIn((39+i*2),LOW,2000)>n){
      s=i;
      n=(pulseIn((39+i*2),LOW,2000));
    }
  }
  int r;
  Serial.print(n);
  if(n==0)  r=12;
  else  r=(IRm[s]);
  Serial.print("\t");
  Serial.print("IRzone:");
  Serial.println(r);
  return (r);
}

int LINE(int zone) {
  if((zone>9 or zone<3) and digitalRead(38)) zone=(zone+6)%12;
  if((zone<6) and digitalRead(40)) zone=(zone+6)%12;
  if((zone<9 and zone>3) and digitalRead(42)) zone=(zone+6)%12;
  if((zone>6) and digitalRead(44)) zone=(zone+6)%12;
  return zone;
}

int getUS(int US){       // берёт значение с ультразвукового датчика
  int value;
  int trig = FIRST_US_PIN+US*2;
  digitalWrite(trig, LOW); //НАЧАЛО ПОЛУЧЕНИЯ ДАННЫХ С US ДАТЧИКА
  delayMicroseconds(5);
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trig, LOW); //КОНЕЦ ПОЛУЧЕНИЯ ДАННЫХ С US ДАТЧИКА
  value = pulseIn(trig+6, HIGH, 20000); //НАЧАЛО ОБРАБОТКИ ПОКАЗАНИЙ US
  value = (value/2) / 29.1;        //КОНЕЦ ОБРАБОТКИ ПОКАЗАНИЙ US
  if(value<=0) value=0;
  Serial.print("          US");
  Serial.print(US);
  Serial.print(":");
  Serial.print(value);
  return value;
}

int SeekerZ(){ //зона с мячом 
  int dir;
  InfraredResult InfraredBall = InfraredSeeker::ReadAC(); //обновление значений
  dir=InfraredBall.Direction; //направление на мяч
  Serial.print("IR direction:                     ");
  Serial.print(dir); //Print the Direction Number
  Serial.print("\t"); //Print a tab
  if((pulseIn(50,LOW,2000))>250){
    switch(dir){
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
        dir=6;
        break;
    }
  }else{
    dir=Seeker[dir];      //направление движения
  }
  Serial.println(dir); 
  return dir;
}
bool CheckButtons(){  //управление режимами при помощи кнопок
  bool D;
  for(int i=0;i<3;i++){
    D=digitalRead(FIRST_BUTTON+i*2);
    if(D!=error[i]){
      error[i]=D;
      if(D){
        state[i]=!state[i];
        state[(i+1)%3]=0;
        state[(i+2)%3]=0;
      } 
    } 
  }
  START=state[0]; //рабочий режим
  DEBUG=state[1]; // режим отладки
  AAA=state[2];
  return true;
}
          
