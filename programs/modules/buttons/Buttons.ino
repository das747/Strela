#define FIRST_BUTTON 48

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

  for(int i=0; i < 3; i++){
    Serial.print(state[i]);
    Serial.print('\t');
  }
  Serial.println();

}
