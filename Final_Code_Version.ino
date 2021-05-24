/* Faaiq Waqar - ECE 342
 * Final Program - Tempterature Sensor Project
 * May 23rd 2021
 */

/* Step One - Import Libraries */
/* Import Library to handle IR signal */
#include <IRremote.h>
/* Import Library for Serial Communication */
#include <SPI.h>
/* Import Library for Radio Communication */
#include<WiFiNINA.h> 
/* Import Library for Blynk GUI Communication*/
#include<BlynkSimpleWiFiNINA.h>
/* Import Library for Temp Sensor Communication*/
#include <Adafruit_MLX90614.h>
/* Import Library for LCD Communication*/
#include <LiquidCrystal.h>   

/* Auth Token in the Blynk App.
 Go to the Project Settings (nut icon). */
char auth[] = "07zFp78wDTHlR_RAi1dYOeCOiwFq6l8L";

/* WiFi credentials.
Set password to "" for open networks. */
char ssid[] = "Linksys12214";
char pass[] = "fyhtse6pr8";

/*-----( Intialize Integers )-----*/
int current_user = 0;
int dig_counter = 0; // Counter for User Entry
int ind = 0; // History Viewing Index
int measurements = 0; // Number of Measurements
int v; // Intermediate Distance Variable

/*-----( Intialize Constants )-----*/
const int receiver = 10;
const int piezoPin = 9;
const int TRIG_PIN = 2; 
const int ECHO_PIN = 3;

/*-----( Intialize Integer Arrays )-----*/
int users[128]; // User Data Over Time

/*-----( Intialize Booleans )-----*/
bool user_set = false; // Is a User Set
bool history = false; // Are we viewing User History

/*-----( Intialize Floats )-----*/
float duration, distance_in_cm;
float temp_obj = 0; // Recorded Temp
float cm; // Distance in cm
float inches; // Distance in inches

/*-----( Intialize Float Arrays )-----*/
float temps[128];  

unsigned long t1;
unsigned long t2;
unsigned long pulse_width;


/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); // init temp sensor
LiquidCrystal lcd(0, 1, 4, 5, 6, 7); // init LCD display

/*----( SETUP: RUNS ONCE )----*/
void setup()
{
  Blynk.begin(auth, ssid, pass); //Initialize IoT
  pinMode(TRIG_PIN, OUTPUT); // Setup Ultrasonic Sensor
  digitalWrite(TRIG_PIN, LOW); 
  
  lcd.begin(16,2); // Prepare initial LCD message
  lcd.print("Enter User ID"); 
  lcd.setCursor(0,2);
  
  irrecv.enableIRIn(); // Start the receiver
  mlx.begin(); // Begin I2C with temp sensor

}/*--(end setup )---*/

/*----( LOOP: RUNS CONSTANTLY )----*/
void loop() 
{
  Blynk.run(); // Start Connectivity to GUI
  if (irrecv.decode(&results)) // have we received an IR signal?

  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */

/*-----( Function: translateIR )-----*/
/*-----( takes action based on IR code received )-----*/
void translateIR()
{

  switch(results.value) //Switch through different buttons
    
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
   if(!history){
   // Hold the trigger pin high for at least 10 us
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
    if(distance_in_cm > 20){
      clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print("Temp:");
        lcd.print("Get Closer!");
    }
    else if(distance_in_cm < 10){
      clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print("Temp:");
      lcd.print("Move Back!");
    }
    
   }while(distance_in_cm > 20 || distance_in_cm < 10);

    // measure and compensate the temperature
    temp_obj = mlx.readObjectTempF() + temp_compensation(distance_in_cm);

    // Notify if a fever is detected
    if(temp_obj >= 100.4){
      tone(piezoPin, 1000, 500);
      clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print("Fever!");
      delay(1000);
    }
   
    // Print out temperature to user
    clearLCDLine(2);
    lcd.setCursor(0,2);
    lcd.print("Temp:");
    lcd.print(temp_obj);
    lcd.print("*F");


    //Send the data to virtual pin 5 wirelessly
    Blynk.virtualWrite(V5, temp_obj); //sending to Blynk

    // Check for overflow, reset if overflow
    if(measurements >= 127) measurements = 0;

    // Add the user data to history log
    users[measurements] = current_user;
    temps[measurements] = temp_obj;
    measurements++;
  }
   
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
    // Case used to intialize reading temperatures
    if (measurements == 0){ //case if no recordings
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
    else{ // if the recordings exist
        if(history){ //are we already viewing the log - yes
          history = false;
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
        
        else{ //are we already viewing the log - no
          history = true;
          lcd.clear();
          lcd.setCursor(0,0); 
          // Print the log and position
          ind = measurements-1;
          lcd.print("Record "); lcd.print(ind+1); lcd.print("/"); lcd.print(measurements);
          lcd.setCursor(0,2);
          lcd.print(users[ind]); lcd.print(" "); lcd.print(temps[ind]); lcd.print("*F");
        }
    }
  break;
  
  case 0xFFE01F: // Move back in log
  if(history && ind > 0){ // Check Bounds
    ind--;
    lcd.clear();
    lcd.setCursor(0,0); 
    // Print welcome to user message and temp
    lcd.print("Record "); lcd.print(ind+1); lcd.print("/"); lcd.print(measurements);
    lcd.setCursor(0,2);
    lcd.print(users[ind]); lcd.print(" "); lcd.print(temps[ind]); lcd.print("*F");
  }
  break;
  
  case 0xFF906F: // Move forward in log
  if(history && ind < measurements-1){ // Check Bounds
    ind++;
    lcd.clear();
    lcd.setCursor(0,0); 
    // Print welcome to user message and temp
    lcd.print("Record "); lcd.print(ind+1); lcd.print("/"); lcd.print(measurements);
    lcd.setCursor(0,2);
    lcd.print(users[ind]); lcd.print(" "); lcd.print(temps[ind]); lcd.print("*F");
  } 
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
    temp_obj = 0;
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

/*----------- temp_compensation: polynomial compensation algorithm ------------*/
float temp_compensation(float distance)
{
  return 0.0039*pow(distance,3) - 0.2231*pow(distance,2) + 4.4830*(distance) - 10.5833;
}
