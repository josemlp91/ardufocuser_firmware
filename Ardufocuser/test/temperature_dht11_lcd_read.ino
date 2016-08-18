#include <DHT11.h>
#include <Wire.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>


#define TEMPIN 7

DHT11 dht11(TEMPIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);


unsigned long  lastTimeUpdate=100;
float temperature;
float humidity;
int error;

void TempController(){
  error=dht11.read(temperature, humidity);
  delay(DHT11_RETRY_DELAY); 
}

void LCDController(){
  lcd.clear();
  lcd.print("TEMP ");
  lcd.print(temperature);
  lcd.print(" C");
  
  lcd.setCursor(0,1);
  lcd.print("HUM  ");
  lcd.print(humidity);
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
    LCDController();
    lastTimeUpdate = millis() + 100;
  }
}



