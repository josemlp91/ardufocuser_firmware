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
  Serial.begin(115200);
  chuck.begin();
  chuck.update();
}

void loop() {
   delay(200);
   chuck.update();
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
