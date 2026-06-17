#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>


#define SERVO_PIN 9
#define greenled 8
#define redled 10
#define buzzer 13

const int rs = 12, en = 11, d4 = A0, d5 = A1, d6 = A2, d7 = A3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


Servo doorServo;

int pos =0;
const unsigned long doorOpenTime = 1000; 
const unsigned long doorClosedTime = 1000;




const byte ROWS = 4; 
const byte COLS = 3; 

// Define the keymap for the keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

// Define the row and column pins connected to the keypad
byte rowPins[ROWS] = {0, 1, 2, 3}; // Connect to the row pins of the keypad
byte colPins[COLS] = {4, 5, 6}; // Connect to the column pins of the keypad

// Create the Keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Define the password
String password = "1234";
String inputPassword = ""; // Variable to store user input


const int LOCKED_POSITION = 0;  
const int UNLOCKED_POSITION = 180; // Unlocked position

void setup() {
  // Initialize the servo
  doorServo.attach(SERVO_PIN);
  doorServo.write(LOCKED_POSITION); // Start with the door locked
  lcd.begin(16, 2);                     
  lcd.print("welcome");
  delay(500);
  lcd.setCursor(0, 2);
  lcd.print("enter password");
  
  

  pinMode(greenled, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
}

void loop() {
  char key = keypad.getKey(); // Get the key pressed

  if (key) { // If a key is pressed

    if (key == '#') { // '#' is used to submit the password
      if (inputPassword == password) {
        lcd.clear();
        lcd.print("Access granted");
        doorServo.write(UNLOCKED_POSITION);
        tone(buzzer, 2000);
        digitalWrite(greenled, HIGH);
        digitalWrite(redled, LOW); // Unlock the door
        
        delay(1000); // Keep the door unlocked for 5 seconds
        noTone(buzzer);
        doorServo.write(LOCKED_POSITION); // Lock the door again
        lcd.clear();
        lcd.print("Door locked.");
        delay(1000);
        lcd.clear();
        lcd.print("occupied");
        digitalWrite(greenled, LOW);
        digitalWrite(redled, LOW);
        

        
      } else {
        lcd.clear();
        lcd.print("Access denied.");
        lcd.setCursor(0, 2);
        for (int i = 0; i< 3; i++) {
          
          tone(buzzer, 2000); 
          delay(200); 
          noTone(buzzer); 
          delay(200); 
        
        }
        lcd.print("wrong password");
        digitalWrite(greenled, LOW);
        digitalWrite(redled, HIGH);
        delay(1000);
        lcd.clear();
        lcd.print("press * to clear");
        digitalWrite(greenled, LOW);
        digitalWrite(redled, LOW);
        

      }
      inputPassword = ""; // Reset the input password
    } else if (key == '*') { // '*' is used to clear the input
      inputPassword = "";
      lcd.clear();
      lcd.print("Input cleared.");
      delay(500);
      lcd.clear();
      lcd.print("enter password");
    } else {
      inputPassword += key; // Append the key to the input password
      Serial.print("Current input: ");
      Serial.println(inputPassword);
    }
  }
}
