#include <EEPROM.h>

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

int MAXCOLORS=7;

int* colorArray[] = {red,green,blue,yellow,cyan,magenta,orange,purple};

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
   
  Serial.print("Max G: ");
  Serial.print(maxG);
  Serial.println("G");
  delay(5);
  
}
