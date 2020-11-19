  #define eyeleds 7
int initialBoot;


void setup() {
    pinMode(eyeleds, OUTPUT);     // declare sensor as output for eyes
    
    initialBoot = 0;
}


void loop()
{

  
  
  //if(initialBoot == 0){
 //  initialBoot = 1;
   digitalWrite(eyeleds, LOW);
   delay(100);
   digitalWrite(eyeleds, HIGH);
   delay(24000);
   
/*  
}else{
     initialBoot = 0;
   digitalWrite(eyeleds, LOW);
   delay(25000);
    
  }
  */
  
  
}
