#define FIRST_SENSOR 39
void setup() {
  Serial.begin(9600);
}

void loop() {
  int IR[12];
  for(int i=0;i<13;i++){
    IR[i]=pulseIn((FIRST_SENSOR+i),LOW,2000);
  }  
  for(int i=0;i<13;i++){
    Serial.print("  D");
    Serial.print(FIRST_SENSOR+i);
    Serial.print(":");
    Serial.print(IR[i]);
  }
  Serial.println();
 }
