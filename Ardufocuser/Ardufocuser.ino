
/*
    This file is part of Ardufocuser Project.

    Ardufocuser is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ardufocuser is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Ardufocuser.  If not, see <http://www.gnu.org/licenses/>.
 */

#define DEBUG 0
#define ENABLE_LCD 1

#include "Arduino.h"
#include <TimerOne.h>
#include <LiquidCrystal_I2C.h>
#include <SerialCommand.h>
#include <AccelStepper.h>
#include <string.h>

#include <Wire.h>
#include <math.h>
#include <nunchuck.h>
#include <EEPROMVar.h>
#include <EEPROMex.h>

#include "Ardufocuser_config.h"
#include "Ardufocuser_init.h"
#include "Ardufocuser_utils.h"
#include "Ardufocuser_cmd.h"

// Ayuda: >> cu -l /dev/ttyACM0  -s 9600

/*
* Bucle principal interrupción software.
*/
void timerFunction() {

	position=motor.currentPosition();

	if (position == limitRunSoftwareA) {
		comunicate_software_limit();

		// Solo permitirmos movimiento en el sentido opuesto al límite.
		if (motor.targetPosition() > limitRunSoftwareA)
			{ motor.run(); limitRunSoftwareActiveA=false;  position=motor.currentPosition(); comunicate_motor_run(); }
		else
			{  motor.moveTo(limitRunSoftwareA);  limitRunSoftwareActiveA=true;}
	}

	if (position == limitRunSoftwareB) {
		comunicate_software_limit();
		// Solo permitirmos movimiento en el sentido opuesto al límite.
		if (motor.targetPosition() < limitRunSoftwareB)
			{ motor.run(); limitRunSoftwareActiveB=false;  position=motor.currentPosition(); comunicate_motor_run(); }

		else
			{ motor.moveTo(limitRunSoftwareB);   limitRunSoftwareActiveB=true; }
	}

	// Si no se cumple ninguna condición permitir girar libremente.
	else {motor.run(); position=motor.currentPosition(); comunicate_motor_run(); }

	// Comunicar posicion.
    comunicate_current_position();
    comunicate_current_temperature();

	// DEBUG:
	//motor.run(); position=motor.currentPosition();
}


/*
* Rutinas que actualiza los controles manuales.
*/
void read_manual_controller(){

	int speed_step;
	int step_step;

  // Si cumple el tiempo minimo de refresco.
  if (millis() > lastTimeReadManualController) {

     int potA = analogRead(PIN_POTA);
     int potB = analogRead(PIN_POTB);

     // Cuando bajoamos el potenciometro al valor 0 forzamos a actualizar la velocidad.
     // Esto se hace así para contemplar la posibilidad de que se esten modificando los valores de forma remota.
     if (potA<=10) { hadToReadspeed=true; }
     
     if (hadToReadspeed){
     	speed = map(potA, 0, 1024, MINVEL, MAXVEL);
     	speed = map(speed, MINVEL, MAXVEL, 0, 21);
     	motor.setMaxSpeed(speed * 10);
     }

     // Cuando bajamos el potenciometro al valor 0 forzamos a actualizar los pasos por pulso.
     // Esto se hace así para contemplar la posibilidad de que se esten modificando los valores de forma remota.
     if (potB<=10) { hadToReadstepPerPulse=true;  }
     
     if (hadToReadstepPerPulse){ 
     	stepPerPulse = map(potB, 0, 1024, 1, MAXSTEPPXPULSA );
     }
   
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
	if (temperature!=last_temperature_displayed){
		lcd.setCursor(0,0);
		lcd.print("TEMP:   ");
		lcd.setCursor(5,0);
		lcd.print(temperature);
		last_temperature_displayed = temperature;
	}

	if (speed!=last_speed_displayed){
		lcd.setCursor(8,1);
		lcd.print("SP:    ");
		lcd.setCursor(12,1);
		lcd.print(speed);
		last_speed_displayed = speed;
	}

	// Dado que comparten el mismo lugar en la pantalla LCD.
	if (button == btnNONE){
		if (stepPerPulse != last_steep_per_pulse_displayed){
			lcd.setCursor(0,1);
			lcd.print("ST:   ");
			lcd.setCursor(3,1);
			lcd.print(stepPerPulse);
			last_steep_per_pulse_displayed = stepPerPulse;
		}
	}
	if (position!=last_position_displayed){
		lcd.setCursor(8,0);
		lcd.print("POS:    ");
		lcd.setCursor(12,0);
		lcd.print(position);
		last_position_displayed = position;
	}
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


// Comunica en tiempo real la posicion exacta del motor (ojo del motor).
void comunicate_current_temperature(){

  if (millis() > lastTimeComunicateTemperature) {
    //int mock_temperature = random(-10, 40);
    String str_temperature(temperature);
    Serial.println("ATEMP?"+ str_temperature);

    
  	lastTimeComunicateTemperature = millis() + current_temperature_refresh_rate;
  }

}

// Comunica en tiempo real la posicion exacta del motor (ojo del motor).
void comunicate_software_limit(){
	
  	if (millis() > lastTimeComunicateSLimit) {
    	Serial.println("ASLIMIT?");
  		lastTimeComunicateSLimit = millis() + current_s_limit_refresh_rate;
  }

}

void comunicate_motor_run(){

	if (millis() > lastTimeComunicateRun) {
    	Serial.println("ARUN?");
  		lastTimeComunicateRun = millis() + current_run_refresh_rate;
  }
}

void temperature_read(){
	if (millis() > lastTimeReadTemp) {
		temperature = analogRead(PIN_TEMSENSOR);
		temperature = (5.0 * temperature * 100.0)/1024.0;
		lastTimeReadTemp = millis() + temp_refresh;
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
		      button=btnRIGHT;
		      lastPulse=btnDOWN;
		      lastTimeReadNunchuckLimit = millis() + nunchuck_refresh_rate;
		  }

		   else if  (chuck.leftJoy()){
		     if (chuck.zPressed()){
		        motor.moveTo(limitRunSoftwareA);
		      }

		      else if (lastPulse!=btnUP){
		        motor.moveTo(motor.currentPosition() - stepPerPulse);
		      }

		        button=btnLEFT;
		        lastPulse=btnUP;
		        lastTimeReadNunchuckLimit = millis() + nunchuck_refresh_rate;
		   }

		   else  {
		   	    button=btnNONE;
			    lastPulse=btnNONE;
			    if (lastTimeReadNunchuckLimit >= millis()) {
	                motor.moveTo(motor.currentPosition()); 
	            }

		   }

		   // Parada de seguridad
		  if (chuck.cPressed()){
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

void save_current_session(){

	// Si el motor se mueve, prorrogamos momento de guardado.
	if (motor.distanceToGo() != 0) {lastTimeSaveSession=millis() + save_sesion_refresh_rate;}
	if (millis() > lastTimeSaveSession) {
		save_session(); // Dentro controlamos que no se hagan guardados de la misma información.
	  	lastTimeSaveSession = millis() + save_sesion_refresh_rate;
	  }
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

  // Actualizamos con datos sesion anterior.
  load_session();

  // iniciamos interrupciones software.
  Timer1.initialize(50);
  Timer1.attachInterrupt( timerFunction );
  
  // Registramos comandos serie.
  registerCommand();
 
}


void loop()
{

  	serial_cmd.readSerial();
  	
  	read_manual_controller();
  	temperature_read();
  	update_lcd_display();
  	nunckuck_controller();
  	//save_current_session();
}




