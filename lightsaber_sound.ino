#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>
#include <TMRpcm.h> 

TMRpcm tmrpcm;   // create an object for use in this sketch

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 10; 

//Define buttons
#define  BUTTON 7
#define  RED 3 //RED connected to digitial pin 9
#define  BLUE 5 //BLUE connected to digitial pin 10
#define  GREEN 6 //GREEN connected to digitial pin 11

//Declare variables
int xRaw=0;
int yRaw=0;
int zRaw=0;
int i=0;
int val = 0;
int old_val=0;

// Raw Ranges:
// initialize to mid-range and allow calibration to
// find the minimum and maximum for each axis
int xRawMin = -512;
int xRawMax = 512;
 
int yRawMin = -512;
int yRawMax = 512;
 
int zRawMin = -512;
int zRawMax = 512;

//Define colors.
int red[] =    {255,0,0};
int green[] =  {0, 255, 0};
int blue[] =   {0,0,255};
int yellow[] = {255,255,0};
int cyan[] = {0,255,255};
int magenta[] = {255,0,255};
int orange[] = {255,165,0};
int purple[] = {128,0,128};
int pink [] = {255,192,203};

int MAXCOLORS=8;

int* colorArray[] = {red,green,blue,yellow,cyan,magenta,orange,purple,pink};

//void clash(){
//  Serial.println("clash!");
//  analogWrite(BLUE,HIGH);
//  analogWrite(GREEN, HIGH);
//  analogWrite(RED, HIGH);
//}

void setup()
{
    pinMode(RED, OUTPUT);  //set digtial pin as output
    pinMode(BLUE, OUTPUT);  //set digtial pin as output
    pinMode(GREEN, OUTPUT);  //set digtial pin as output
    pinMode(BUTTON, INPUT); //Cycle colors
    Serial.begin(9600);
    analogReference(EXTERNAL);
    if (EEPROM.read(0) != 255){
       i = EEPROM.read(0);
    }else {
      i = 0;
    }

    // if analog input pin 3 is unconnected, random analog
    // noise will cause the call to randomSeed() to generate
    // different seed numbers each time the sketch runs.
    // randomSeed() will then shuffle the random function.
    randomSeed(analogRead(3));
    Serial.print("\nInitializing SD card...");
    pinMode(SS, OUTPUT);
    
     while (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
  } 
  
  // print the type of card
  Serial.print("\nCard type: ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();
  
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);

  
  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
  tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  tmrpcm.play("SABERON.WAV");
}

void loop()
{
  int* currentColor=colorArray[i];
  //Serial.println(colorArray[0]);
  analogWrite(RED, currentColor[0]);
  analogWrite(GREEN, currentColor[1]);
  analogWrite(BLUE, currentColor[2] );
  
  delay(50);
  
  int val = digitalRead(BUTTON);
  
  if ((val == HIGH) && (old_val == LOW)){
     i=i+1;
     if( i>MAXCOLORS){
        i = 0;
     }
     EEPROM.write(0,i);
     delay(500);
     tmrpcm.play("HUM.WAV");
  }
  
  //Read in Accelerometer
  
  xRaw=analogRead(A1);
  yRaw=analogRead(A2);
  zRaw=analogRead(A3);
  
    // Convert raw values to 'milli-Gs"
    long xScaled = map(xRaw, xRawMin, xRawMax, -1000, 1000);
    long yScaled = map(yRaw, yRawMin, yRawMax, -1000, 1000);
    long zScaled = map(zRaw, zRawMin, zRawMax, -1000, 1000);
  
    // re-scale to fractional Gs
    float xAccel = xScaled / 1000.0;
    float yAccel = yScaled / 1000.0;
    float zAccel = zScaled / 1000.0;
    
    float maxG = max(xAccel, yAccel);
    maxG = max(maxG,zAccel);
    
    
    //Serial.print(" Max G: ");
    //Serial.print(maxG);
    //Serial.println("G");
  
  delay(5);
  
  //
  
  
}
