// Ayuda: >> cu -l /dev/ttyACM0  -s 9600

#define DEBUG 0
#define ENABLE_LCD 1



#include <TimerOne.h>
#include <LiquidCrystal_I2C.h>
#include <SerialCommand.h>
#include <AccelStepper.h>

#include <Wire.h>
#include <math.h>
#include <nunchuck.h>
#include <EEPROM.h>

#include "Ardufocuser_config.h"
#include "Ardufocuser_init.h"
#include "Ardufocuser_cmd.h"
#include "Ardufocuser_utils.h"


int count=0;

void setup()
{
  Serial.begin(4800);
  lcd.begin();
  lcd.backlight();

  registerCommand();
}


void loop()
{
  	serial_cmd.readSerial();

	if (millis() > lastTimeUpdate) {
	  lcd.clear();
      lcd.print(count);
      
	  lastTimeUpdate = millis() + refresh_rate;
	  count+=1;
	}
}




