
#include <LiquidCrystal.h>            
// includes the LiquidCrystal Library 
#include <IRremote.h>
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);  
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 
int receiver = 11;           
// Set the digital pin
IRrecv irrecv(receiver);     
// create instance of 'irrecv'
decode_results results;      
// create instance of 'decode_results'
int current_user = 0;  
// saves the user pin of the current user     
int dig_counter = 0;
// saves the digit that the user ID entry is in
bool user_set = false;
// indicates whether a user is logged in
float temp = 0.0;
// saves the latest measured temperature
bool measured = false;
// indicated whether a temperature has been recorded before


/*-----------Setup: Prepare Objects, Variables and LCD for Main Loop ------------*/

void setup() { 
 lcd.begin(16,2); 
 // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display }
 irrecv.enableIRIn(); 
 // Start the receiver 
 lcd.print("Enter User ID"); 
 // Prints "Arduino" on the LCD
 lcd.setCursor(0,2);
 // Set the curser to the second line
  
}

/*-----------Loop: Make a simple loop that polls for IR input ------------*/
void loop() { 
  if (irrecv.decode(&results)) 
  // have we received an IR signal?

  {
    translateIR(); 
    irrecv.resume(); 
    // receive the next value
  } 
}

/*-----------translateIR: Make use of IR signal for user input ------------*/

void translateIR() // takes action based on IR code received

// describing Remote IR codes 

{
switch(results.value)
    
  {

  case 0xFF02FD: 
  // Pause Button Entry Correlating with New User
    user_set = false;
    current_user = 0;
    dig_counter = 0;
    // Reset the user parameters
    lcd.clear();
    lcd.print("Enter User ID"); 
    // Prints  on the LCD
    lcd.setCursor(0,2); 
    break;
 
  case 0xFF30CF:
  // "1" Button Entry     
      if (user_set == false){
      current_user += 1*quick_pow10(dig_counter);
      dig_counter++;
      clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
    }
  break;
  case 0xFF18E7: 
  // "2" Button Entry        
      if (user_set == false){
      current_user += 2*quick_pow10(dig_counter);
      dig_counter++;
      clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
    }
  break;
  case 0xFF7A85:
  // "3" Button Entry        
      if (user_set == false){
      current_user += 3*quick_pow10(dig_counter);
      dig_counter++;
      clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
    }
  break;
  case 0xFF10EF:
  // "4" Button Entry         
      if (user_set == false){
      current_user += 4*quick_pow10(dig_counter);
      dig_counter++;
      clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
    }
  break;
  case 0xFF38C7:
  // "5" Button Entry         
      if (user_set == false){
      current_user += 5*quick_pow10(dig_counter);
      dig_counter++;
      clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
    }
  break;
  case 0xFF5AA5:
  // "6" Button Entry         
      if (user_set == false){
      current_user += 6*quick_pow10(dig_counter);
      dig_counter++;
      clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
    }
  break;
  case 0xFF42BD:
  // "7" Button Entry         
      if (user_set == false){
      current_user += 7*quick_pow10(dig_counter);
      dig_counter++;
      clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
    }
  break;
  case 0xFF4AB5:
  // "8" Button Entry         
      if (user_set == false){
      current_user += 8*quick_pow10(dig_counter);
      dig_counter++;
      clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
    }
  break;
  case 0xFF52AD:
  // "9" Button Entry         
      if (user_set == false){
      current_user += 9*quick_pow10(dig_counter);
      dig_counter++;
      clearLCDLine(2);
      lcd.setCursor(0,2);
      lcd.print(current_user); 
      // Prints on the LCD
    }
  break;
  default:
    return;

  }// End Case

  // If the user entry is complete, exit entry
  if (dig_counter == 4){
    user_set = true;
    lcd.clear();
    lcd.setCursor(0,0); 
    // Print welcome to user message and temp
    lcd.print("Welcome:"); // Prints "Arduino" on the LCD
    lcd.print(current_user);
    lcd.setCursor(0,2);
    lcd.print("Temp:");
    if(measured == false) lcd.print("N/A");
    else lcd.print(temp);
  }

  delay(500);
  // Do not allow rapid entry
}


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
