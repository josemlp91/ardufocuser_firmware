#include <Wire.h>
#include <AccelStepper.h>
#include <LiquidCrystal_I2C.h>

AccelStepper motor(1, PINSTEP, PINDIR);
LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.print("LINE 1");
  lcd.setCursor(1,1);
  lcd.print("LINE 2");
  delay(2000);
}

void loop() {
   motor.run();
   motor.moveTo(3000);
}
