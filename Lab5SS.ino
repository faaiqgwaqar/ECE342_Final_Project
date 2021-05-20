#include <Adafruit_MLX90614.h>

//Create variables to use later
const int TRIG_PIN = 9; 
const int ECHO_PIN = 10;
float temp_obj = 0;
float duration, distance_in_cm;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup()
{
   //Setup pins for correct I/O
   pinMode(TRIG_PIN, OUTPUT); 
   pinMode(ECHO_PIN, INPUT); 
   //TODO: Add Serial Monitor communication setup
   Serial.begin(9600);
   mlx.begin();
}

void loop()
{
   do{
   //Generate the ultrasonic waves
   digitalWrite(TRIG_PIN, LOW); 
   delayMicroseconds(2); 
   digitalWrite(TRIG_PIN, HIGH); 
   delayMicroseconds(10); 
   digitalWrite(TRIG_PIN, LOW);

  //Read in the echoed waves
   duration = pulseIn(ECHO_PIN, HIGH);

   //Convert from time to distance using the speed of sound
   
    distance_in_cm = (duration*.0343)/2;
   }while(distance_in_cm > 30 || distance_in_cm < 10);
   
   temp_obj = mlx.readObjectTempF() + temp_compensation(distance_in_cm);

   
   //TODO: Print out the distance to the Serial Monitor
   Serial.println(distance_in_cm); Serial.println(" "); Serial.println(temp_obj);

   delay(2000);
   
}

float temp_compensation(float distance)
{
  return 0.0039*pow(distance,3) - 0.2231*pow(distance,2) + 4.4830*(distance) - 10.5833;
}

/*
 * Matlab Script Testing
x = [10 11 15 17 18 20 25];
y = [81.89 79.11 77.55 77.31 76.66 75.61 74.43];
y = 97 - y;

p = polyfit(x,y,3)
x2 = 1:.1:30;
y2 = polyval(p,x2);
plot(x2,y2)
grid on
s = sprintf('y = (%.1f) x^3 + (%.1f) x^2 + (%.1f) x + (%.1f)',p(1),p(2),p(3),p(4));
text(2,400,s)*/
