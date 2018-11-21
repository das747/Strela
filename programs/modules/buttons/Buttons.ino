#define FIRST_BUTTON 26

bool error[]={0,0,0};
bool state[]={0,0,0};


void setup() {
Serial.begin(9600);

}

void loop() {

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


Serial.print(START);
Serial.print("\t");
Serial.print(DEBUG);
Serial.print("\t");
Serial.println(AAA);

}
