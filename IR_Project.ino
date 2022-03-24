#include <IRremote.h>            // Imclude IR remote library
#include<LiquidCrystal.h>        // Inclue LCD library
#include "pitches.h"
LiquidCrystal lcd(4, 5, 6, 7, 8, 9); // Use these GPIOs to conrol the LCD
int RECV_PIN = 2;                // IR receiver input line is GPIO 2
long stored;                     // Long integer "stored" will be used to store large amounts of data.  This is a large integer, essentially.  It can hold 32 bits (4 bytes), from -2,147,483,648 to 2,147,483,647.
long storedd;                    // Second long integer "storedd". use these integers to store IR protocol data
long buf;

boolean check = false;

IRrecv irrecv(RECV_PIN);         // Requred - Set up IR receiver pin in software
decode_results results;          // Required for software - results will store IR data



#define buzzer 1
int time = 0;
int hold = 0;

IRsend irsend;                   // Enables IR transmit on pin 3
unsigned int irLen = 0;          // The length of the code
unsigned int irBuffer[RAWBUF];   // Storage for raw (UNKNOWN) codes.

void setup()
{
  pinMode(buzzer, OUTPUT);
  lcd.begin(16, 2);               // Set up LCD
  lcd.clear();                    // Clear LCD
  irrecv.enableIRIn();   // Start the receiver
  lcd.setCursor(3, 0);
  lcd.print("IR Music");  // Write to top line of LCD
  lcd.setCursor(4, 1);
  lcd.print("Player");
}


// https://create.arduino.cc/projecthub/executeli/unravel-preset-piano-easy-arduino-even-a-ghoul-can-make-it-17c472?ref=tag&ref_id=piano&offset=2
// Credit to song translator
// notes in the melody:
int melody[] = {
  NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_G4,  NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_F4, 0, 0,
  NOTE_DS4, NOTE_DS4, NOTE_F4, NOTE_D4, 0, 0, 0, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D5, NOTE_D5,
  NOTE_G3, NOTE_AS3, NOTE_C4, NOTE_G3, NOTE_G3, NOTE_AS3, NOTE_AS4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_AS4, NOTE_AS4,
  NOTE_G3, NOTE_AS3, NOTE_C4, NOTE_G3, NOTE_G3, NOTE_AS3,


  NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_G4,  NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_F4, 0, 0,
  NOTE_DS4, NOTE_DS4, NOTE_F4, NOTE_D4, NOTE_G3, NOTE_AS3, NOTE_C4, NOTE_F3, NOTE_AS3, NOTE_A3, NOTE_F3,
  NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D5, NOTE_D5,
  NOTE_G3, NOTE_AS3, NOTE_C4, NOTE_F3, NOTE_AS3, NOTE_G3, NOTE_AS4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_AS4, NOTE_AS4,
};

String melodyStr[] = {
  "AS4", "C5", "AS4", "A4", "G4",  "C5", "AS4", "A4", "G4", "G4", "F4", "0", "0",
  "DS4", "DS4", "F4", "D4", "0", "0", "0", "D4", "D4", "D4", "D4", "D5", "D5",
  "G3", "AS3", "C4", "G3", "G3", "AS3", "AS4", "A4", "A4", "A4", "AS4", "AS4",
  "G3", "AS3", "C4", "G3", "G3", "AS3",


  "AS4", "C5", "AS4", "A4", "G4",  "C5", "AS4", "A4", "G4", "G4", "F4", "0", "0",
  "DS4", "DS4", "F4", "D4", "G3", "AS3", "C4", "F3", "AS3", "A3", "F3",
  "D4", "D4", "D4", "D5", "D5",
  "G3", "AS3", "C4", "F3", "AS3", "G3", "AS4", "A4", "A4", "A4", "AS4", "AS4",
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {

  4.5, 2.25, 2.25, 4.5, 2.25, 2.25, 2.25, 2.25, 2.25, 4.5, 3, 9, 4.5,
  4.5, 2.25, 4.5, 2.25, 1.125, 2.25, 4.5, 4.5, 2.25, 4.5, 2.25, 4.5, 2.25,
  4.5, 4.5, 4.5, 2.25, 4.5, 4.5, 4.5, 2.25, 4.5, 2.25, 4.5, 2,
  4.5, 4.5, 4.5, 2.25, 4.5, 4.5,

  4.5, 2.25, 2.25, 4.5, 2.25, 2.25, 2.25, 2.25, 2.25, 4.5, 3, 9, 4.5,
  4.5, 2.25, 4.5, 4.5, 4.5, 4.5, 4.5, 4.5, 3, 3, 4.5,
  2.25, 4.5, 2.25, 4.5, 2.25,
  4.5, 4.5, 4.5, 4.5, 2.25, 4.5, 4.5, 2.25, 4.5, 2.25, 4.5, 2.25,
} ;



void loop() {
  // Start of main program
  if (check == false) {
    if (irrecv.decode(&results)) {  // Wait for incoming IR stream from remote
      check = true;
      lcd.clear();
      lcd.print("Music playing");
      lcd.setCursor(0, 2);
      irrecv.resume();

      for (int thisNote = 0; thisNote < 85; thisNote++) {
        lcd.clear();
        lcd.print("Music playing");
        lcd.setCursor(0, 2);
        lcd.print(melodyStr[thisNote]);
        int noteDuration = 600 / noteDurations[thisNote];
        tone(buzzer, melody[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.50;
        delay(pauseBetweenNotes);
        noTone(buzzer);
        if (irrecv.decode(&results)) {
          thisNote = 85;
        }
      }
    }
  }
  if (irrecv.decode(&results)) {
    lcd.clear();
    lcd.print("Music playing");
    lcd.setCursor(0, 2);

    irrecv.resume();
    for (int thisNote = 0; thisNote < 85; thisNote++) {
      lcd.clear();
      lcd.print("Music playing");
      lcd.setCursor(0, 2);
      lcd.print(melodyStr[thisNote]);
      int noteDuration = 600 / noteDurations[thisNote];
      tone(buzzer, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.50;
      delay(pauseBetweenNotes);
      noTone(buzzer);
      if (irrecv.decode(&results)) {
        thisNote = 85;
      }
    }
  }
}
