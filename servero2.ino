// this sketch cycles three servos at different rates 

#include <ServoTimer2.h>  // the servo library

// define the pins for the servos
#define rollPin  24
#define pitchPin 3
#define yawPin   4

ServoTimer2 servoRoll;    // declare variables for up to eight servos
ServoTimer2 servoPitch; 
ServoTimer2 servoYaw; 
  int val;
  int pos;
void setup() {
  servoRoll.attach(rollPin);     // attach a pin to the servos and they will start pulsing
//  servoPitch.attach(pitchPin); 
 // servoYaw.attach(yawPin); 
 
 val = 0;
 pos = 2000;
}


// this function just increments a value until it reaches a maximum 
int incPulse(int val, int inc){
   if( val + inc  > 2000 )
      return 800 ;
   else
       return val + inc;  
}

void loop()
{ 

// 

  
  
  if(pos == 800){
  pos = 2000;
  }else {
    pos = 800;
  }
  


  
  // pos = map(val, 0, 180, 750 , 2250 ); 
  
  // val = incPulse( servoRoll.read(), 1);

   //
   servoRoll.write(pos);
Serial.println(pos, DEC);
   //val =  incPulse( servoPitch.read(), 2);
   //servoPitch.write(val);
   
   //val = incPulse(servoYaw.read(), 4);
   //servoYaw.write(val);
   
   delay(550);   
}
