

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROMVar.h>
#include <EEPROMex.h>



struct RecControlConfig {
  int  speed;
  int stepPerPulse;
  int position;
  int temperature;
  int aceleration;
  int microStep;
  bool lcd_lux_status;
  int mode;
  int count;

};

int count;
RecControlConfig rec_config;
int address = 0;
int lastTimeUpdate;
LiquidCrystal_I2C lcd(0x27, 16, 2);



void save_random_register()
{

  randomSeed(analogRead(0));

  Serial.println("CREANDO  ENTRADAS");
  rec_config.speed = random(0, 100);
  rec_config.stepPerPulse = random(0, 100);
  rec_config.position = random(0, 100);
  rec_config.temperature = random(0, 100);
  rec_config.aceleration = random(0, 100);
  rec_config.microStep = random(0, 100);
  rec_config.lcd_lux_status = random(0, 1);
  rec_config.mode = random(0, 100);
  rec_config.count = count;

  Serial.println(rec_config.count);

  EEPROM.writeBlock(address, rec_config);

}


void load_last_sesion(){
  EEPROM.readBlock(address, rec_config);
  count=rec_config.count;
}


void setup()
{


Serial.begin(9600);
lcd.begin();
lcd.backlight();
load_last_sesion();

}

void loop()
{

  if (millis() > lastTimeUpdate) {
      count++;
      lcd.clear();
      lcd.print(count);
      save_random_register();
      lastTimeUpdate = millis() + 1000;
  }

}
