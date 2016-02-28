// Ayuda: >> cu -l /dev/ttyACM0  -s 9600

#define DEBUG 0
#define ENABLE_LCD 1

#include <TimerOne.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SerialCommand.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);
SerialCommand serial_cmd; 

unsigned long lastTimeUpdate=0;
int refresh_rate=1000;

int count=0;   

void setup()
{
  Serial.begin(4800);
  lcd.begin();
  lcd.backlight();

  serial_cmd.addCommand("GETCOUNT", function_GETCOUNT);
  serial_cmd.addCommand("SETCOUNT", function_SETCOUNT);

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


void  function_GETCOUNT()
{
	char charBuf[50];
	String str_position;
	String str_cmd;
  
	str_position = String(count);

	str_cmd = String("AG?" + str_position );
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd);	
}



void function_SETCOUNT() 
{
	
	char charBuf[50];
	String str_position;
	String str_cmd;
	int aNumber;
	char *arg;  

	arg = serial_cmd.next();

  if (arg != NULL) {
    aNumber = atoi(arg);

    count=aNumber;

    str_position = String(count);
    str_cmd = String("AG?" + str_position );
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 

  }
  else {
    Serial.println("No arguments");
  }

}

