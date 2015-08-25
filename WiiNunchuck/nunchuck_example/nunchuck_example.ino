#include <Wire.h>
#include <math.h>
#include "nunchuck.h"
//#include "nunchuck_funcs.h"

#define MAXANGLE 90
#define MINANGLE -90


WiiChuck chuck = WiiChuck();
int angleStart, currentAngle;
int tillerStart = 0;
double angle;

void setup() {
  
    pinMode(13, OUTPUT);  
  //nunchuck_init();
  Serial.begin(115200);
  chuck.begin();
  chuck.update();
  //chuck.calibrateJoy();
}


void loop() {
  delay(200);
  chuck.update(); 


  //Serial.print(chuck.readRoll());
  //Serial.print(", ");  
  //Serial.print(chuck.readPitch());
  //Serial.print(", ");  

   // Serial.print((int)chuck.readAccelX()); 
   // Serial.print(", ");  
   // Serial.print((int)chuck.readAccelY()); 
   // Serial.print(", ");  

   // Serial.print((int)chuck.readAccelZ()); 
   
   Serial.print((int)chuck.leftJoy()); 
   Serial.print(", ");   
   Serial.print((int)chuck.rightJoy());
   Serial.print("----"); 
   Serial.print((int)chuck.zPressed());
   Serial.print(", "); 
   Serial.print((int)chuck.cPressed());
   
   
   if (chuck.rightJoy()){
     
     digitalWrite(13, HIGH);
   
   
   }
   
   else {
   
   digitalWrite(13, LOW);
   }

    Serial.println();

}
