#include "SoftwareSerial.h"
#include "MP3FLASH16P.h"
MP3FLASH16P myPlayer;

const int buttonPin4 = 4; 
const int buttonPin5 = 5; 
const int buttonPin6 = 6; 
const int buttonPin7 = 7; 
const int buttonPin8 = 8; 
const int buttonPin9 = 9; 
const int buttonPin10 = 10; 
const int led = 13; 
    
int songIndex = 0;
void setup() 
{
    pinMode(buttonPin4, INPUT);
    pinMode(buttonPin5, INPUT);
    pinMode(buttonPin6, INPUT);
    pinMode(buttonPin7, INPUT);
    pinMode(buttonPin8, INPUT);
    pinMode(buttonPin9, INPUT);
    pinMode(buttonPin10, INPUT);
    pinMode(led, OUTPUT);
    myPlayer.init(3);
   // Serial.begin(9600);
}

void loop() 
{
  // read the state of the pushbutton value:
   if (digitalRead(buttonPin4) == HIGH ) {
      songIndex = 6;
   } else if(digitalRead(buttonPin5) == HIGH){
      songIndex = 7;
   } else if(digitalRead(buttonPin6) == HIGH){
     songIndex = 1;
   } else if(digitalRead(buttonPin7) == HIGH){
      songIndex = 2;    
   } else if(digitalRead(buttonPin8) == HIGH){
      songIndex = 3;    
   } else if(digitalRead(buttonPin9) == HIGH){
     songIndex = 4;
   } else if(digitalRead(buttonPin10) == HIGH){
      songIndex = 5;    
   }

   if(songIndex > 0){
      digitalWrite(led, HIGH); 
      myPlayer.playFileAndWait(songIndex, 15);
      digitalWrite(led, LOW);
      songIndex = 0;
   }

 }
