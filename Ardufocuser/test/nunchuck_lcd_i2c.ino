#include <Wire.h>
#include <math.h>
#include <nunchuck.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);
WiiChuck chuck = WiiChuck();

const int btnRIGHT=0;
const int btnUP=1;
const int btnDOWN=2; 
const int btnLEFT=3;
const int btnC=4; 
const int btnZ=5; 
const int btnNONE=6;

int lastTimeUpdate=1000;
int lastPulse;

void nunckuckController(){

  chuck.update();

  if(chuck.cPressed() && lastPulse!=btnC ){
    lcd.clear();
    lcd.print("C");
    lastPulse=btnC;
                
	}if(chuck.zPressed() && lastPulse!=btnZ){
    lcd.clear();
    lcd.print("Z");
    lastPulse=btnZ;
                
	}if(chuck.rightJoy() && lastPulse!=btnRIGHT){
    lcd.clear();
    lcd.print("RIGHT");
    lastPulse=btnRIGHT;
	
  }if(chuck.leftJoy()&& lastPulse!=btnLEFT){
    lcd.clear();
    lcd.print("LEFT");
    lastPulse=btnLEFT;
	
  }else  {
    lastPulse=btnNONE;
	}

}

void setup() {

  Serial.begin(9600);
  lastPulse=btnNONE;
  lcd.begin();
  lcd.backlight();
  lcd.print("HELLO!");
  chuck.begin();
  chuck.update();


}


void loop() {
  if (millis() > lastTimeUpdate) {
    nunckuckController();
    lastTimeUpdate = millis();
  }
}

