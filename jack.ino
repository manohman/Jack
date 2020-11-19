
#include <avr/pgmspace.h>
#include <WaveHC.h>
#include <WaveUtil.h>
#include <ServoTimer2.h>  // the servo library

#define DEBOUNCE 100
#define NUM_FILES 28
#define swPin 14
#define eyeleds 26
#define MOUTH_CLOSED  775
#define MOUTH_MID  1500
#define MOUTH_OPEN  2225
// Number of files.
#define FILE_COUNT 50

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time
FatReader file;   // This object represent the WAV file 

uint8_t dirLevel; // indent level for file/dir names    (for prettyprinting)
dir_t dirBuf;     // buffer for directory reads

int inputPin = 22;               // choose the input pin (for PIR sensor)
int servoPin = 24;              // choose the input pin (for Servo)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status for motion sensor
int minPulse     =  600;  // minimum servo position
int maxPulse     =  2200; // maximum servo position
int turnRate     =  1800;  // servo turn rate increment (larger value, faster rate)
int refreshTime  =  20;   // time (ms) between pulses (50Hz)
int mouthchange = 6;  //checks to see if mouth position needs to be changed
int randNumber;   //random number variable to allow the choosing of which wav will be played
int actualFileCount;
int currentFileIndex;
                // a maximum of eight servo objects can be created 

/** The Arduino will calculate these values for you **/
int centerServo;         // center servo position
int pulseWidth;          // servo pulse width
long lastPulse   = 0;    // recorded time (ms) of the last pulse
int pos = 0;    // variable to store the servo position 
  
int mouthPosition;
ServoTimer2 servoRoll;    // declare variables for up to eight servos

int initialBoot;
// time to play each tone in milliseconds
#define PLAY_TIME 200

/*
 * Define macro to put error messages in flash memory
 */
#define error(msg) error_P(PSTR(msg))

// Function definitions (we define them here, but the code is below)
void play(FatReader &dir);


uint16_t fileIndex[FILE_COUNT];
char* fileName[FILE_COUNT];

void setup() {
  // set up serial port
  Serial.begin(9600);
 
  pinMode(inputPin, INPUT);     // declare sensor as input for PIR
  pinMode(eyeleds, OUTPUT);     // declare sensor as output for eyes

 servoRoll.attach(servoPin);     // attach a pin to the servos and they will start pulsing

//   set up servo pin
//  pinMode(servoPin, OUTPUT);  // Set servo pin 18 (analog 4) as an output pin
//  centerServo = maxPulse - ((maxPulse - minPulse)/2);
//  pulseWidth = centerServo;   // Give the servo a starting point (or it floats)


  putstring_nl("\nWave test!");  // say we woke up!
  
  putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(FreeRam());

  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    error("Card init. failed!");  // Something went wrong, lets print out why
  }
  
  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);
  
  // Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {   // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                           // we found one, lets bail
  }
  if (part == 5) {                     // if we ended up not finding one  :(
    error("No valid FAT partition!");  // Something went wrong, lets print out why
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(), DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    error("Can't open root dir!");      // Something went wrong,
  }
  
  // Whew! We got past the tough parts.
  putstring_nl("Files found (* = fragmented):");

  // Print out all of the files in all the directories.
  root.ls(LS_R | LS_FLAG_FRAGMENTED);
  actualFileCount = 0;
 val = 0;
 pos = 2000;
 initialBoot = 1;
 currentFileIndex = 0;
  root.rewind();
 indexFiles(root);
 
 
 currentFileIndex = 20;
 
}

void loop()
{

   if(initialBoot == 1){
      digitalWrite(eyeleds, HIGH);
      servoRoll.write(MOUTH_CLOSED);

      Serial.println("Initializing IR Motion Sensor, 45 seconds...");
      delay(25000);      
      Serial.println("20 seconds...");
      delay(10000);
      Serial.println("10 seconds...");
      delay(5000);
      Serial.println("5 seconds...");
      delay(5000);
      Serial.println("...initialized!");

      initialBoot = 0;
     digitalWrite(eyeleds, LOW);
  }

  
  val = digitalRead(inputPin);


  if (val == HIGH)
  {          
    if (pirState == LOW)
    {
      // we have just turned on
   //  Serial.println("Motion!");
     //Turn eyes on
     digitalWrite(eyeleds, HIGH);
     // Play a sound: 
//     randNumber = 0;
//     randNumber = random(actualFileCount);
     
     if(currentFileIndex > 5000){
       currentFileIndex = 0;
     }
     
     int playIndex = currentFileIndex++ % actualFileCount;
     
      if (playIndex >= 0)
      {
             pirState = HIGH;
    
//           char * name = GetWavName(28);
//                      char * name = GetWavName(playIndex);
                      
// Serial.println(name);
 // Serial.println(playIndex);
 //   Serial.println(currentFileIndex);
  //                    
//           playcomplete(name);
 
//  Serial.println("Playing index ");
//  Serial.println(playIndex, DEC);
             playCompleteByIndex(playIndex);
      }
    }
  } else  {
    if (pirState == HIGH)
    {
      digitalWrite(eyeleds, LOW);
      // we have just turned off
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;

     }
  }

 }

/////////////////////////////////// HELPERS
/*
 * print error message and halt
 */
void error_P(const char *str) {
  PgmPrint("Error: ");
  SerialPrint_P(str);
  sdErrorCheck();
  while(1);
}

/*
 * print error message and halt if SD I/O error, great for debugging!
 */
void sdErrorCheck(void) {
  if (!card.errorCode()) return;
  PgmPrint("\r\nSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  PgmPrint(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

void playcomplete(char *name)
{
  char i;
  uint8_t volume;
  int v2;


  playByName(name);
 
  while (wave.isplaying)
  {
    
  
            if(mouthPosition == 0){    
             pos = MOUTH_OPEN;
               mouthPosition = 1;
            }
               else
            {
               pos = MOUTH_CLOSED;
                         mouthPosition = 0; 
            }
 
      servoRoll.write(pos);
 
 
     delay(400);  
  }
 
 
 servoRoll.write(MOUTH_CLOSED);
 
  //card.close_file(f);
  digitalWrite(eyeleds, LOW);
  // we have just turned off 
}

void playCompleteByIndex(int index)
{
  char i;
  uint8_t volume;
  int v2;


  playByIndex(index);
 
  while (wave.isplaying)
  {
    
  
            if(mouthPosition == 0){    
             pos = MOUTH_OPEN;
               mouthPosition = 1;
            }
               else
            {
               pos = MOUTH_CLOSED;
                         mouthPosition = 0; 
            }
 
      servoRoll.write(pos);
 
 
     delay(400);  
  }
 
 
 servoRoll.write(MOUTH_CLOSED);
 
  //card.close_file(f);
  digitalWrite(eyeleds, LOW);
  // we have just turned off 
}


/*
void playfile(char *name)
{
  // stop any file already playing
  if (!wave.isplaying)
  {
    f = card.open_file(name);
    if (f && wave.create(f))
    {
      wave.play();
    }
  }


} 
*/




/*
 * Play file by name and print latency in ms
 */
void playByName(char *name) {
  if (!wave.isplaying){
//  char name[10];
  
  // copy flash string to RAM
  //strcpy_P(name, PSTR("DTMFx.WAV"));
  
//  for (uint8_t i = 0; i < FILE_COUNT; i++) {
    // start time
    uint32_t t = millis();
    
    // make file name
//    name[4] = fileLetter[i];
    
    // open file by name
    if (!file.open(root, name)) error("open by name"); 
    
    // create wave and start play
    if (!wave.create(file))error("wave.create");
    wave.play();
    
    // print time
   // Serial.println(millis() - t);
    
    // stop after PLAY_TIME ms
  //  while((millis() - t) < PLAY_TIME);
//    wave.stop();
    
    // check for play errors
    sdErrorCheck();
  //}
//  PgmPrintln("Done");
  }
}

void playByIndex(int index) {
  if (!wave.isplaying){
//  char name[10];
  
  // copy flash string to RAM
  //strcpy_P(name, PSTR("DTMFx.WAV"));
  
//  for (uint8_t i = 0; i < FILE_COUNT; i++) {
    // start time
    uint32_t t = millis();
    
    // make file name
//    name[4] = fileLetter[i];
    
    
   //  Serial.print("Opening file: ");
     //    Serial.println(fileName[index]);
    // open file by name
    if (!file.open(root, fileIndex[index])) error("open by index"); 
    
    
    
    
    // create wave and start play
    if (!wave.create(file))error("wave.create");
    wave.play();
    
    // print time
   // Serial.println(millis() - t);
    
    // stop after PLAY_TIME ms
  //  while((millis() - t) < PLAY_TIME);
//    wave.stop();
    
    // check for play errors
    sdErrorCheck();
  //}
  PgmPrintln("Done");
  }
}


void indexFiles(FatReader &dir) {
  
  
    FatReader file;
  int index = 0;
  while (dir.readDir(dirBuf) > 0) {    // Read every file in the directory one at a time
  
      // Skip it if not a subdirectory and not a .WAV file
    if (!DIR_IS_SUBDIR(dirBuf)
         && strncmp_P((char *)&dirBuf.name[8], PSTR("WAV"), 3)) {
      continue;
    }
  
      if (!file.open(vol, dirBuf)) {        // open the file in the directory
      error("file.open failed");          // something went wrong
    }
    
      // Save file's index (byte offset of directory entry divided by entry size)
    // Current position is just after entry so subtract one.
    fileIndex[index++] = root.readPosition()/32 - 1;  
//    fileName[index - 1] = (char *)&dirBuf.name;
  actualFileCount++;
  
  //Serial.print(index- 1);
 // Serial.print(" - ");
//Serial.println((char *)&dirBuf.name);
  
  }
  
  
  /*
  char name[10];
  
  // copy flash string to RAM
  strcpy_P(name, PSTR("DTMFx.WAV"));
  
  for (uint8_t i = 0; i < FILE_COUNT; i++) {
    
    // Make file name
    name[4] = fileLetter[i];
    
    // Open file by name
    if (!file.open(root, name)) error("open by name");
    
    // Save file's index (byte offset of directory entry divided by entry size)
    // Current position is just after entry so subtract one.
    fileIndex[i] = root.readPosition()/32 - 1;   
  }
  */
   Serial.println("IndexFile complete ");
   Serial.println(actualFileCount, DEC);
  PgmPrintln("Done");
}




char * GetWavName(int index){

               if (index == 1)
               {
                 return "1.WAV";
               }
             
              else if (index == 2)
              {
                  return "2.wav";
              }
             
              else if (index == 3)
              {
                  return "3.WAV";
              }
              else if (index == 4)
              {
                  return "4.WAV";
              }

              else if (index == 5)
              {
                  return "5.WAV";
              }

              else if (index == 6)
              {
                  return "6.WAV";
              }

              else if (index == 7)
              {
                  return "7.WAV";
              }

              else if (index == 8)
              {
                  return "8.WAV";
              }

              else if (index == 9)
              {
                  return "9.WAV";
              }

              else if (index == 10)
              {
                  return "10.WAV";
              }

              else if (index == 11)
              {
                  return "11.WAV";
              }

              else if (index == 12)
              {
                  return "12.WAV";
              }

              else if (index == 13)
              {
                  return "13.WAV";
              }

              else if (index == 14)
              {
                  return "14.WAV";
              }

              else if (index == 15)
              {
                  return "15.WAV";
              }

              else if (index == 16)
              {
                  return "16.WAV";
              }

              else if (index == 17)
              {
                  return "17.WAV";
              }

              else if (index == 18)
              {
                  return "18.WAV";
              }

              else if (index == 19)
              {
                  return "19.WAV";
              }

              else if (index == 20)
              {
                  return "20.WAV";
              }

              else if (index == 21)
              {
                  return "21.WAV";
              }
              else if (index == 22)
              {
                  return "22.WAV";
              }
  
              else if (index == 23)
              {
                  return "23.WAV";
              }
  
                else if (index == 24)
              {
                  return "24.WAV";
              }

              else if (index == 25)
              {
                  return "26.WAV";
              }
              else if (index == 26)
              {
                  return "26.WAV";
              }    

              else if (index == 27)
              {
                  return "27.WAV";
              }    

              else if (index == 28)
              {
                  return "28.WAV";
              }    
                  return "1.WAV";

}


