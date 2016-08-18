#include <Wire.h>
#include <math.h>
#include <nunchuck.h>
#include <LiquidCrystal_I2C.h>

#define MIN 0
#define MAX 100

#define TEMPIN 2

LiquidCrystal_I2C lcd(0x27, 16, 2);


unsigned long  lastTimeUpdate=100;
int lastPulse;
float temperature;


void TempController(){

  int result, result2;
  temperature = analogRead(TEMPIN);
  
   temperature = (5.0 * temperature * 100.0)/1024.0;
  

  lcd.clear();
  lcd.print(temperature);
  lcd.print("  ");


}

void setup() {

  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.print("RUN!");


}


void loop() {

  if (millis() > lastTimeUpdate) {
    TempController();
    lastTimeUpdate = millis() + 100;


  }
}
