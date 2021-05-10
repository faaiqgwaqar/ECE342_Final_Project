int ThermistorPin = 0;
int Vo;
float R1 = 10000;
float logR2, R2, T;


void setup() {
pinMode(9, OUTPUT);
Serial.begin(9600);
}

void loop() {

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  T = (T * 9.0)/ 5.0 + 32.0; 
  if(T >= 100.4){
     tone(9, 1000, 1000);
  }
  
  Serial.print("Temperature: "); 
  Serial.print(T);
  Serial.println(" F"); 

  delay(250);
}
