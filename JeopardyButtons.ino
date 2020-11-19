
const int buttonPin4 = 4; 
const int buttonPin5 = 5; 
const int buttonPin6 = 6; 

const int buttonPin7 = 7; 
const int buttonPin8 = 8; 
const int buttonPin9 = 9; 
    
//int songIndex = 0;
void setup() 
{
    pinMode(buttonPin4, INPUT);
    pinMode(buttonPin5, INPUT);
    pinMode(buttonPin6, INPUT);
    
    pinMode(buttonPin7, OUTPUT);
    pinMode(buttonPin8, OUTPUT);
    pinMode(buttonPin9, OUTPUT);
    
   Serial.begin(9600);

   Serial.println("Starting up"); // Print the vol
}

void loop() 
{
  // read the state of the pushbutton value:
   if (digitalRead(buttonPin4) == HIGH ) {
      Serial.println("Pin4"); // Print the vol
      LightUp(buttonPin7); 
   } else if(digitalRead(buttonPin5) == HIGH){
        Serial.println("Pin5"); // Print the vol
        LightUp(buttonPin8); 
   } else if(digitalRead(buttonPin6) == HIGH){
      Serial.println("Pin6"); // Print the vol
      LightUp(buttonPin9); 
   } 
   
   /*
   else if(digitalRead(buttonPin7) == HIGH){

   } else if(digitalRead(buttonPin8) == HIGH){

   } else if(digitalRead(buttonPin9) == HIGH){

   } else if(digitalRead(buttonPin10) == HIGH){

   }
*/

}
void LightUp(int pinNumber)
{

      digitalWrite(pinNumber, HIGH); 
      delay(3000);
     digitalWrite(pinNumber, LOW); 

}



 
