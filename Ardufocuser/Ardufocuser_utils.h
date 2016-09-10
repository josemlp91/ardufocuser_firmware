
bool char_is_ascii_letter(char c)
{
	return  (c >= int('a') || c <=int('z'));
}

bool char_is_ascii_number(char c)
{
	return  (c >= int('1') || c <=int('9'));
}

bool char_is_ascii_letter_or_number(char c)
{
	return (char_is_ascii_letter(c) || char_is_ascii_number(c));
}


int string_to_int(String c)
{
	return c.toInt();
}

// Set  high o low en pines correspondientes a ...............................................................................................................//(MS1, MS2, MS3)
void setMicroFullStep()     { digitalWrite(PINMICROSTEP_MS1,LOW);   digitalWrite(PINMICROSTEP_MS2,LOW);   digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=1; } //( 0,   0,   0 )
void setMicroHalfStep()     { digitalWrite(PINMICROSTEP_MS1,HIGH);  digitalWrite(PINMICROSTEP_MS2,LOW);   digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=2; } //( 1,   0,   0 )
void setMicroQuarterStep()  { digitalWrite(PINMICROSTEP_MS1,LOW);   digitalWrite(PINMICROSTEP_MS2,HIGH);  digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=3; } //( 0,   1,   0 )
void setMicroEighthStep()   { digitalWrite(PINMICROSTEP_MS1,HIGH);  digitalWrite(PINMICROSTEP_MS2,HIGH);  digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=4; } //( 1,   1,   0 )
void setMicroSixteenthStep(){ digitalWrite(PINMICROSTEP_MS1,HIGH);  digitalWrite(PINMICROSTEP_MS2,HIGH);  digitalWrite(PINMICROSTEP_MS3,HIGH); microStep=5; } //( 1,   1,   1 )
void setMicro(int micro_mode)
{
	switch (micro_mode) {
	    case 1:
	     	setMicroFullStep();
	      	break;
	    case 2:
	      	setMicroHalfStep();
	      	break;
	    case 3:
	      	setMicroQuarterStep();
	      	break;
	    case 4:
	      	setMicroEighthStep();
	      	break;
	    case 5:
	      	setMicroSixteenthStep();
	    	break;

	    default:
		 	setMicroSixteenthStep();
	    break;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MANEJO DE LAS SESIONES.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Almacenamos la sesion en memoria eeprom, cuando proceda.
void save_session()
{
	RecControlConfig aux_rec_config;
	EEPROM.readBlock(EEPROM_SESSION_ADDRESS, aux_rec_config);

	// Ejecutamos rutina de guardado solo cuando se hayan modificado los valores.
	if ( // Comprobamos si se ha modificado algún valor.
		//(aux_rec_config.speed != speed) ||
		//(aux_rec_config.stepPerPulse  != stepPerPulse) || 
		(aux_rec_config.position  != position) ||
		//(aux_rec_config.temperature  != temperature) ||
		//(aux_rec_config.aceleration  != aceleration) ||
		(aux_rec_config.microStep  != microStep) ||
		//(aux_rec_config.lcd_lux_status  != lcd_lux_status) ||
		(aux_rec_config.mode  != mode)
	){

		// DEBUG:
		Serial.println("GUARDENDO"); 		
		
		// Cargamos las variables en la estructura session
		rec_config.speed = speed;
		rec_config.stepPerPulse = stepPerPulse ;
		rec_config.position = position;
		rec_config.temperature = 30;
		rec_config.aceleration = aceleration;
		rec_config.microStep = microStep;
		rec_config.lcd_lux_status = lcd_lux_status;
		rec_config.mode = mode;

		EEPROM.writeBlock(EEPROM_SESSION_ADDRESS, rec_config);

	} 
}

// Cargamos sesion anterior almacenada en eeprom.
void load_session(){

	// Cargamos sesion anterior.
	EEPROM.readBlock(EEPROM_SESSION_ADDRESS, rec_config);

	speed=rec_config.speed;
	stepPerPulse=rec_config.stepPerPulse;
	position=rec_config.position;
	temperature=rec_config.temperature;
	aceleration=rec_config.aceleration;
	microStep=rec_config.microStep;
	lcd_lux_status=rec_config.lcd_lux_status;
	mode=rec_config.mode;

	// Aplicamos configuracion
	motor.setCurrentPosition(position);
	setMicro(microStep);
	motor.setMaxSpeed(speed);
    motor.setAcceleration(aceleration);
    lcd.setBacklight(lcd_lux_status);

}