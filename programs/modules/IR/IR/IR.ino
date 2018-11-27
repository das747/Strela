#define FIRST_IR 23
void setup() {
  Serial.begin(9600);
}

void loop() {
  int IR[15];
  for(int i = 0; i < 15; i++){
    IR[i]=pulseIn((FIRST_IR + (i * 2))),LOW,2000);
  }  
  for(int i = 24; i <= 52; i += 2){
    Serial.print("  D");
    Serial.print(i);
    Serial.print(":");
    Serial.print(IR[i]);
  }
  Serial.println();
 }
