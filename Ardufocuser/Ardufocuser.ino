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

/*
* Bucle principal interrupción software.
*/
void timerFunction() {

	position=motor.currentPosition();
	if (position == limitRunSoftwareA) {
		// Solo permitirmos movimiento en el sentido opuesto al límite.
		if (motor.targetPosition() > limitRunSoftwareA)
			{ motor.run(); limitRunSoftwareActiveA=false;  position=motor.currentPosition();}
		else
			{  motor.moveTo(limitRunSoftwareA);  limitRunSoftwareActiveA=true;}
	}

	if (position == limitRunSoftwareB) {
		// Solo permitirmos movimiento en el sentido opuesto al límite.
		if (motor.targetPosition() < limitRunSoftwareB)
			{ motor.run(); limitRunSoftwareActiveB=false;  position=motor.currentPosition(); }

		else
			{ motor.moveTo(limitRunSoftwareB);   limitRunSoftwareActiveB=true; }
	}

	// Si no se cumple ninguna condición permitir girar libremente.
	else {motor.run(); position=motor.currentPosition();}

	// DEBUG:
	//motor.run(); position=motor.currentPosition();
}


/*
* Rutinas que actualiza los controles manuale.
*/
void read_manual_controller(){

  // Si cumple el tiempo minimo de refresco.
  if (millis() > lastTimeReadManualController) {

     int potA = analogRead(PIN_POTA);
     int potB = analogRead(PIN_POTB);

     // Cuando bajoamos el potenciometro al valor 0 forzamos a actualizar la velocidad.
     // Esto se hace así para contemplar la posibilidad de que se esten modificando los valores de forma remota.
     
     if (potA==0) hadToReadspeed=true;
     if (hadToReadspeed){ speed = map(potA, 0, 1024, MINVEL, MAXVEL ); motor.setMaxSpeed(speed); }

     // Cuando bajamos el potenciometro al valor 0 forzamos a actualizar los pasos por pulso.
     // Esto se hace así para contemplar la posibilidad de que se esten modificando los valores de forma remota.
    
     if (potB==0) hadToReadstepPerPulse=true;
     if (hadToReadstepPerPulse){ stepPerPulse = map(potB, 0, 1024, 1, MAXSTEPPXPULSA); }

     lastTimeReadManualController = millis() + manual_controller_refresh_rate;
    }
}

// Refresca LCD
void update_lcd_display(){

  // Si cumple el tiempo minimo de refresco.
   if (millis() > lastTimeUpdateLCD) {

    // Si ha cambiado el boton desde la última pulsación.
    if (button != lastPulse)  {
      // actualizamos zona LCD donde se muestra el comando.
      lcd.setCursor(0,1);
      switch (button){
        case btnRIGHT:  lcd.print("RIGHT  ");  break;
        case btnLEFT:   lcd.print("LEFT   ");  break;
        case btnUP:     lcd.print("RIGHT_F");  break;
        case btnDOWN:   lcd.print("LEFT_F ");  break;
        case btnSELECT: lcd.print("OK     ");  break;

      // Actualizamos boton anterior al valor del boton actual.
      lastPulse=button;

      }
    }

	// Actualizamos zona de la temperatura
	lcd.setCursor(0,0);
	lcd.print("TEMP:   ");
	lcd.setCursor(5,0);
	lcd.print(temperature);

	lcd.setCursor(8,1);
	lcd.print("SP:    ");
	lcd.setCursor(12,1);
	lcd.print(speed);


	lcd.setCursor(0,1);
	lcd.print("ST:   ");
	lcd.setCursor(3,1);
	lcd.print(stepPerPulse);


	lcd.setCursor(8,0);
	lcd.print("POS:    ");
	lcd.setCursor(12,0);
	lcd.print(position);
	
	// Refrescamos momento de actualizacion.
	lastTimeUpdateLCD = millis() + lcd_refresh_rate;

  }

}

// Comprobar si ha cambiado la temperatura un valor suficientemente grande.
bool is_change_thermal_optical(){
	// Si cumple el tiempo minimo de refresco.
	if (millis() > lastTimeCheckChangeTemp) {

		if ((temperature>temp_perform+MAX_CHANGE_TEMP) || (temperature<temp_perform-MAX_CHANGE_TEMP)){ return true; }
		else { return false; }
	lastTimeCheckChangeTemp = millis() + temp_refresh_rate;

	}else{ return false; }
}

// Activa led de aviso.
void enable_warning() { digitalWrite(PINLED_WARNING,HIGH); }

// Desactiva led de aviso.
void disable_warning(){ digitalWrite(PINLED_WARNING,LOW); }

// Comunica en tiempo real la posicion exacta del motor (ojo del motor).
void comunicate_current_position(){

  if (millis() > lastTimeComunicatePosition) {

    String current_position(motor.currentPosition());
    Serial.println("APOSITION?"+ current_position);

    if (motor.distanceToGo() == 0) { Serial.println("ASTOP?"); }
  	lastTimeComunicatePosition = millis() + current_position_refresh_rate;
  }

}


// Controlador WiiNunckuck para Ardufocuser
void nunckuck_controller(){

  chuck.update();
  if (chuck.rightJoy()){
    if (chuck.zPressed()){
      motor.moveTo(limitRunSoftwareB);
    }

    else if (lastPulse!=btnDOWN){
          motor.moveTo(motor.currentPosition() + stepPerPulse);
    }
      lastPulse=btnDOWN;
  }

   else if  (chuck.leftJoy()){
     if (chuck.zPressed()){
        motor.moveTo(limitRunSoftwareA);
      }

      else if (lastPulse!=btnUP){
        motor.moveTo(motor.currentPosition() - stepPerPulse);
      }
        lastPulse=btnUP;
   }

   else  {
    lastPulse=btnNONE;
    motor.moveTo(motor.currentPosition());
   }

}


//saludo inicial.
void welcome(char* msg)
{
  lcd.setCursor(0,0);
  lcd.print(msg);
  delay(1000);
  lcd.setCursor(0,0);
  lcd.clear();
}



void setup()
{

  Serial.begin(9600);
  
  // iniciamos lcd
  lcd.begin();
  lcd.backlight();
  
  // saludo
  welcome("   ARDUFOCUSER  ");

  // velocidad y aceletación por defecto.
  motor.setMaxSpeed(200);
  motor.setAcceleration(1000);
  
  //Iniciamos nunckuck
  chuck.begin();
  chuck.update();

  // iniciamos interrupciones software.
  Timer1.initialize(50);
  Timer1.attachInterrupt( timerFunction );
  
  registerCommand();
 
}


void loop()
{
  	serial_cmd.readSerial();
  	read_manual_controller();
  	update_lcd_display();
  	//comunicate_current_position();
  	//nunckuck_controller();

}




