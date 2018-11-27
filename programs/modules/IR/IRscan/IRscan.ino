#define FIRST_SENSOR 24

const int IRpins[] = {38, 36, 34,  32, 30, 28,  26, 24, 46,  44, 42, 23,  25, 27, 40};
void setup() {
  Serial.begin(9600);
}

void loop() {
  int IR[15];
  for(int i = 0; i < 15; i++){
    IR[i]=pulseIn((IRpin[i]),LOW,2000);
  }  
  for(int i = 0; i < 15; i++){
    Serial.print("  D");
    Serial.print(IRpin[i]);
    Serial.print(":");
    Serial.print(IR[i]);
  }
  Serial.println();
 }
