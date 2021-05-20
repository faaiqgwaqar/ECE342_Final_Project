/* Faaiq Waqar - ECE 342
 * Block Checkoff 2 Demo - Radio Communications
 * May 11th 2021
 */

 // Step One - Import Libraries used for Communication
#include <IRremote.h>
#include <SPI.h>
#include<WiFiNINA.h> 
#include<BlynkSimpleWiFiNINA.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal.h>   

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Estabolish Thermistor Constants to for Conversion

// Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "07zFp78wDTHlR_RAi1dYOeCOiwFq6l8L";

// WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Linksys12214";
char pass[] = "fyhtse6pr8";

int current_user = 0;  
int users[128];
// saves the user pin of the current user     
int dig_counter = 0;
// saves the digit that the user ID entry is in
bool user_set = false;
bool history = false;
int measurements = 0;


// indicates whether a user is logged in
float temp_obj = 0;
float temp_amb = 0;
float temps[128];


int receiver = 10; // Signal Pin of IR receiver to Arduino Digital Pin 11
int piezoPin = 9;
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);  


const int TRIG_PIN = 12;
const int ECHO_PIN = 11;


 unsigned long t1;
 unsigned long t2;
 unsigned long pulse_width;
 float cm;
 float inches;
 int v;
 float w;

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Blynk.begin(auth, ssid, pass);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  lcd.begin(16,2); 
 // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display }
  lcd.print("Enter User ID"); 
 // Prints "Arduino" on the LCD
  lcd.setCursor(0,2);
 // Set the curser to the second line
  irrecv.enableIRIn(); // Start the receiver
  mlx.begin();

}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  Blynk.run();
  if (irrecv.decode(&results)) // have we received an IR signal?

  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */

/*-----( Function )-----*/
void translateIR() // takes action based on IR code received

// describing Remote IR codes 

{

  switch(results.value)
    
  {

  case 0xFF02FD:  
  // Case for play button - Input User Begins
    user_set = false;
    current_user = 0;
    dig_counter = 0;
    // Reset the user parameters
    lcd.clear();
    lcd.print("Enter User ID"); 
    // Prints  on the LCD
    lcd.setCursor(0,2); 
    // Prints  on the Serial
  break;

  case 0xFF6897: 
      // Hold the trigger pin high for at least 10 us
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_PIN, LOW);

  // Wait for pulse on echo pin
      while ( digitalRead(ECHO_PIN) == 0 );

      // Measure how long the echo pin was held high (pulse width)
      // Note: the micros() counter will overflow after ~70 min
      t1 = micros();
      while ( digitalRead(ECHO_PIN) == 1);
      t2 = micros();
      pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed
  //of sound in air at sea level (~340 m/s).
      cm = pulse_width / 58.0;

    // Serial.print("Distance [cm] = "); Serial.print(cm); Serial.print("cm\n");
    
    temp_obj = mlx.readObjectTempF();
  
   //  Serial.print("Ambient = "); Serial.print(temp_amb); 
    // Serial.print("*F\tObject = "); Serial.print(temp_obj); Serial.println("*F");
    clearLCDLine(2);
    lcd.setCursor(0,2);
    lcd.print("Temp:");
    //Blynk user connected by virtual pin 3
    lcd.print(temp_obj);
    lcd.print("*F");

    if(temp_obj >= 100.2) tone(piezoPin, 1000, 500);

    //Send the data to virtual pin 5 wirelessly
    Blynk.virtualWrite(V5, temp_obj); //sending to Blynk
  break;

  // Numpad Buttons - Input to user ID
  case 0xFF30CF: //1
      if (!user_set && !history){
        current_user += 1*quick_pow10(dig_counter);
        dig_counter++;
              clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
      } 
  break;
  
  case 0xFF18E7: //2
      if (!user_set && !history){
        current_user += 2*quick_pow10(dig_counter);
        dig_counter++;
             clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
      }    
  break;
  
  case 0xFF7A85: //3
      if (!user_set && !history){
        current_user += 3*quick_pow10(dig_counter);
        dig_counter++;
              clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
      }    
  break;
  
  case 0xFF10EF: //4
      if (!user_set && !history){
        current_user += 4*quick_pow10(dig_counter);
        dig_counter++;
           clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
      }       
  break;
  
  case 0xFF38C7: //5
      if (!user_set && !history){
        current_user += 5*quick_pow10(dig_counter);
        dig_counter++;
             clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
      }       
  break;
  
  case 0xFF5AA5: //6
      if (!user_set && !history){
        current_user += 6*quick_pow10(dig_counter);
        dig_counter++;
              clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
      }      
  break;
  
  case 0xFF42BD: //7
      if (!user_set && !history){
        current_user += 7*quick_pow10(dig_counter);
        dig_counter++;
              clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
      }       
  break;
  
  case 0xFF4AB5: //8
      if (!user_set && !history){
        current_user += 8*quick_pow10(dig_counter);
        dig_counter++;
              clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
      }        
  break;
  
  case 0xFF52AD: //9
      if (!user_set && !history){
        current_user += 9*quick_pow10(dig_counter);
        dig_counter++;
              clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
      }       
  break;

  case 0xFFA857: 
    if (measurements == 0){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("No Recordings!");
        delay(1000);

        lcd.clear();
        lcd.setCursor(0,0); 
        // Print welcome to user message and temp
        lcd.print("Welcome:"); // Prints "Arduino" on the LCD
        lcd.print(current_user);
        lcd.setCursor(0,2);
        lcd.print("Temp:");
        //Blynk user connected by virtual pin 3
        lcd.print(temp_obj);
        lcd.print("*F");
    }
    else{
        if (history) history = false;
        
        else{
          history = true;
          lcd.clear();
          lcd.setCursor(0,0); 
          // Print welcome to user message and temp
          lcd.print("Record "); lcd.print(measured); lcd.print("/"); lcd.print(measured);
          lcd.setCursor(0,2);
          lcd.print(users[measured -1]); lcd.print(" "); lcd.print(temps[measured -1]); lcd.print("*F");
        }
    }
  break;
  
  case 0xFFE01F:
  Serial.println("DOWN");    
  break;
  
  case 0xFF906F: 
  Serial.println("UP");    
  break;
  

  }// End Case

  // When the user entry is complete, set the username (4 digit)
  if (dig_counter == 4){
    //set user to true
    user_set = true;
    // add one to get out of this case
    dig_counter = 5;
    // Serial Welcomes the user - test
    lcd.clear();
    lcd.setCursor(0,0); 
    // Print welcome to user message and temp
    lcd.print("Welcome:"); // Prints "Arduino" on the LCD
    lcd.print(current_user);
    lcd.setCursor(0,2);
    lcd.print("Temp:");
    Blynk.virtualWrite(V3, current_user); //sending to Blynk
    //Blynk user connected by virtual pin 3
    lcd.print(temp_obj);
    lcd.print("*F");
  }

  delay(500); // Do not get immediate repeat


} //END translateIR

/*----------- quick_pow10: quick method to get powers of 10 ------------*/
static int quick_pow10(int n)
{
    static int pow10[4] = {
        1, 10, 100, 1000
    };

    return pow10[n]; 
}

/*----------- clearLCDLine: quickly clear a single line ------------*/
void clearLCDLine(int line)
{               
        lcd.setCursor(0,line);
        for(int n = 0; n < 16; n++) // 20 indicates symbols in line. For 2x16 LCD write - 16
        {
                lcd.print(" ");
        }
}
