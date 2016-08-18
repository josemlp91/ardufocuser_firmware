#include <Wire.h>
#include <math.h>
#include <nunchuck.h>
#include <LiquidCrystal_I2C.h>

#define MIN 0
#define MAX 100

LiquidCrystal_I2C lcd(0x27, 16, 2);


unsigned long  lastTimeUpdate=100;
int lastPulse;

void PotController(){

  int result, result2;
  int pot = analogRead(A0);
  int pot2 = analogRead(A1);
  result = map(pot, 0, 1024, MIN, MAX );
  result2 = map(pot2, 0, 1024, MIN, MAX );

  lcd.clear();
  lcd.print(result);
  lcd.print("  ");
  lcd.print(result2);

}

void setup() {

  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.print("RUN!");


}


void loop() {

  if (millis() > lastTimeUpdate) {
    PotController();
    lastTimeUpdate = millis() + 100;


  }
}
