#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float temp_obj = 0;
float temp_amb = 0;
int ledPin9 = 9;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin9, OUTPUT); 
  digitalWrite(ledPin9, LOW);
  mlx.begin();  
}

void loop() {
  temp_obj = mlx.readObjectTempF();
  temp_amb = mlx.readAmbientTempF();
  
  Serial.print("Ambient = "); Serial.print(temp_amb); 
  Serial.print("*F\tObject = "); Serial.print(temp_obj); Serial.println("*F");

  if(temp_obj >= 99.00 && temp_obj <= 101.00){
    digitalWrite(ledPin9, HIGH);
    delay(500);
  }
  else{digitalWrite(ledPin9, LOW);}
 

  Serial.println();
  delay(500);
}
