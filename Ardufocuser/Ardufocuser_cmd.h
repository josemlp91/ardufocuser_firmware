//Definimos los indices de cada comando.
//OJO: Para añadir un comando debemos asegurarnos que lo añadimos en la posición que corresponde con su indice.


// Instancia de SerialCommand, tokenizador y parseador de puerto serie.
SerialCommand serial_cmd; 


#define AINIT   				0    // Iniciar modo funcionamiento Ardufocuser.
#define AMODE   				1    // Estableceer modo de funcionamiento.
#define AG   		  			2    // Mover enfocador hasta posición pasada como dato.
#define APOSITION 			    3    // Solicitar posición actual del enfocador.
#define ATEMP   				4    // Solicitar temperatura de la lente al enfocador.
#define AMICRO   				6    // Establecer multiplicación del micropaso.
#define AFINE   				7    // Establecer pasos que da el enfocador por cada pulso.
#define ASPEED   				8    // Establecer la velocidad del movimiento del enfocador.
#define AACC   					9    // Establecer la aceleración del movimiento del enfocador.
#define AR   		  		   10    // Fijar posición reletiva del enfocador en un valor personalizado.
#define AHLIMIT   		       11    // Consultar si el enfocador ha llegado a un limite hardware.
#define	ASLIMIT				   12	 // Consultar si el enfocador ha llegado a un limite software.
#define ASILIMIT  		       13    // Establecer  limite software inware.
#define ASOLIMIT  		       14    // Establecer  limite software outware.
#define AVERS   			   15    // Consultar version del firmware.
#define AMOV   				   16    // Consulta si el enfocador esta moviendose.
#define ARUNA   	           17    // Comando para debug: Mueve el motor del enfocador en una dirreción.
#define ARUNB   			   18    // Comando para debug: Mueve el motor del enfocador en una dirreción.
#define ASTOP   			   19    // Comando para debug: Detiene motor del enfocador
#define ALUX   	               20    // Modifica la intesindad de la ilumincaicon de la pantalla.
#define ACALIBRATE   	       21    // Configura los limites software.
#define ALCDPRINT   	       22    // Imprime mensaje en lcd


// Set  high o low en pines correspondientes a ...............................................................................................................//(MS1, MS2, MS3)
void setMicroFullStep()     { digitalWrite(PINMICROSTEP_MS1,LOW);   digitalWrite(PINMICROSTEP_MS2,LOW);   digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=1; } //( 0,   0,   0 )
void setMicroHalfStep()     { digitalWrite(PINMICROSTEP_MS1,HIGH);  digitalWrite(PINMICROSTEP_MS2,LOW);   digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=2; } //( 1,   0,   0 )
void setMicroQuarterStep()  { digitalWrite(PINMICROSTEP_MS1,LOW);   digitalWrite(PINMICROSTEP_MS2,HIGH);  digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=3; } //( 0,   1,   0 )
void setMicroEighthStep()   { digitalWrite(PINMICROSTEP_MS1,HIGH);  digitalWrite(PINMICROSTEP_MS2,HIGH);  digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=4; } //( 1,   1,   0 )
void setMicroSixteenthStep(){ digitalWrite(PINMICROSTEP_MS1,HIGH);  digitalWrite(PINMICROSTEP_MS2,HIGH);  digitalWrite(PINMICROSTEP_MS3,HIGH); microStep=5; } //( 1,   1,   1 )
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Ajusta modo autofocus.
void ardufocuser_command_function_AINIT()
{
   char charBuf[50];
   String str_mode;
   String str_cmd;

   // Establece modo de funcionamiento.   
   mode = ARDUFOCUS_MODE;
   
   str_mode = String(mode);
   str_cmd = String("AINIT?" + str_mode );
   str_cmd.toCharArray(charBuf, 50); 
   Serial.println(str_cmd); 

}

void ardufocuser_command_function_ALCD_PRINT()
{
    char charBuf[50];
    char *arg;  
	String str_cmd;
	
	arg = serial_cmd.next();

  if (arg != NULL) {
   
    lcd.clear();
    lcd.print(arg);
    str_cmd = String("ALCDPRINT");
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 

  }
  else { Serial.println("No arguments"); }
}

void ardufocuser_command_function_AMODE()
{
	
	char charBuf[50];
	String str_mode;
	String str_cmd;

	int aNumber;
	char *arg;  

	arg = serial_cmd.next();

	if (arg != NULL) {
		aNumber = atoi(arg);
		mode = aNumber;
		str_mode = String(mode);
	}

	else { str_mode = String(mode); }

	str_cmd = String("AMODE?" + str_mode );
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd);

}


// Mueve motor hasta la psocición pasada como parametro.
void ardufocuser_command_function_AG()
{
	
	char charBuf[50];
	String str_position;
	String str_cmd;
	int aNumber;
	char *arg;  

	arg = serial_cmd.next();

	if (arg != NULL) {
		
		aNumber = atoi(arg);
		position = aNumber;
		str_position = String(position);	
	}

	else{ str_position = String(position); }

	str_cmd = String("AG?" + str_position);
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 

}


void ardufocuser_command_function_APOSITION()
{
   char charBuf[50];
   String str_cmd;
   String str_position=String(position);

   str_cmd = String("APOSITION?" + str_position );
   str_cmd.toCharArray(charBuf, 50); 
   Serial.println(str_cmd); 
}


// Envia mensaje informando de la temperatura del sensor.
void ardufocuser_command_function_ATEMP(void){
   
   char charBuf[50];
   String str_cmd;
   String str_temperature=String(temperature);
   	
   str_cmd = String("ATEMP?" + str_temperature );
   str_cmd.toCharArray(charBuf, 50); 
   Serial.println(str_cmd); 
}


// Ajusta numero de paso en movimiento paso a paso.
void ardufocuser_command_function_AFINE(){
	
   	char charBuf[50];
	String str_cmd;
	String str_step_per_pulse;
	int aNumber;
	char *arg;  

	arg = serial_cmd.next();

	if (arg != NULL) {
		aNumber = atoi(arg);
		if (aNumber>MAXSTEPPXPULSA){ stepPerPulse=MAXSTEPPXPULSA; }
    	else if (aNumber<1){  stepPerPulse=1; }
    	else{ stepPerPulse = aNumber; }
		str_step_per_pulse=String(stepPerPulse);

		// TODO: 
		// hadToReadstepPerPulse=false;
	}

	else{ str_step_per_pulse=String(stepPerPulse); } 
	str_cmd = String("AFINE?" + str_step_per_pulse );
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 
}


// Ajusta la velocidad de movimiento del motor.
void ardufocuser_command_function_ASPEED(){

	char charBuf[50];
	String str_cmd;
	String str_speed;
	int aNumber;
	char *arg;  

	arg = serial_cmd.next();

	if (arg != NULL) {

		aNumber = atoi(arg);
		if (aNumber>MAXVEL) aNumber=MAXVEL;
        else if (aNumber<MINVEL) aNumber=MINVEL;
    	else{ speed = aNumber; }
		str_speed=String(speed);

		// TODO: SET SPEED moto.setSpeed....
		// hadToReadspeed=false, para no hacer caso a los potenciometros
	}

	else{ str_speed=String(speed);} 
	str_cmd = String("ASPEED?" + str_speed );
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 
}


// Ajusta la aceleración del motor.
void ardufocuser_command_function_AACC(void){

	char charBuf[50];
	String str_cmd;
	String str_aceleration;
	int aNumber;
	char *arg;  

	arg = serial_cmd.next();

	if (arg != NULL) {

		aNumber = atoi(arg);
		if (aNumber>MAXACC) aNumber=MAXACC;
        else if (aNumber<MINACC) aNumber=MINACC;
    	else{ aceleration = aNumber; }
		str_aceleration=String(aceleration);

		motor.setAcceleration(aceleration);
		//TODO: SET SPEED moto.setSpeed....
	}

	else{ str_aceleration=String(aceleration);} 
	str_cmd = String("AACC?" + str_aceleration );
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 
}


// Ajusta posición actual como posición pasada por argumento.
void ardufocuser_command_function_AR(){

	char charBuf[50];
	String str_position;
	String str_cmd;
	int aNumber;
	char *arg;  

	arg = serial_cmd.next();

  if (arg != NULL) {
    aNumber = atoi(arg);
    position=aNumber;
    str_position = String(position);
  }

  else {
     str_position = String(position);
  }

  	str_cmd = String("AR?" + str_position );
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 
}


// Consulta si estamos en alguna posición final de recorrido.
void ardufocuser_command_function_AHLIMIT(){

	char charBuf[50];
	String str_cmd;
	String str_hlimitA=String(limitRunActiveA);
	String str_hlimitB=String(limitRunActiveB);
	

	str_cmd = String("AHLIMIT?" + str_hlimitA + "-" + str_hlimitB );
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 

}

// Consulta si el motor se esta moviendo.
void ardufocuser_command_function_AMOV(){

	char charBuf[50];
	String str_cmd;

	// Consultar si el enfocador ha llegado a la posicion destino o si se esta moviendo).
    if (motor.distanceToGo() == 0) { str_cmd = String("ASTOP?");}
    else {str_cmd = String("ASTOP?");}

    Serial.println(str_cmd); 

}


// Realiza configuración de micropasos, diviendo cada paso en la mitad, cuarto, octava o dieciseisava.
void ardufocuser_command_function_AMICRO(){

	char charBuf[50];
	String str_cmd;
	String str_micro_step;
	int Imicro;
	char *arg;  

	arg = serial_cmd.next();

	if (arg != NULL) {
		Imicro = atoi(arg);		
			switch (Imicro) {

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
 
    str_micro_step=String(microStep);
	str_cmd = String("AMICRO?" + str_micro_step);
    Serial.println(str_cmd); 
}



// Consuta limistes software
void ardufocuser_command_function_ASLIMIT(){
	char charBuf[50];
	String str_cmd;
	String str_slimitA=String(limitRunSoftwareActiveA);
	String str_slimitB=String(limitRunSoftwareActiveB);
	
	str_cmd = String("ASLIMIT?" + str_slimitA + "-" + str_slimitB );
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 
}



// Ajusta limite software A.
void ardufocuser_command_function_ASILIMIT(){
	char charBuf[50];
	String str_silimit;
	String str_cmd;
	int aNumber;
	char *arg;  

	arg = serial_cmd.next();

  if (arg != NULL) {
    aNumber = atoi(arg);
    limitRunSoftwareA=aNumber;
    str_silimit = String(limitRunSoftwareA);
  }

  else {
     str_silimit = String(limitRunSoftwareA);
  }
  	str_cmd = String("ASILIMIT?" + str_silimit );
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 
}

// Ajusta limite software B.
void ardufocuser_command_function_ASOLIMIT(){

	char charBuf[50];
	String str_solimit;
	String str_cmd;
	int aNumber;
	char *arg;  

	arg = serial_cmd.next();

  if (arg != NULL) {
    aNumber = atoi(arg);
    limitRunSoftwareB=aNumber;
    str_solimit = String(limitRunSoftwareB);
  }

  else {
     str_solimit = String(limitRunSoftwareB);
  }

  	str_cmd = String("ASOLIMIT?" + str_solimit );
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 
}



// Modifica brillo de la pantalla LCD
void ardufocuser_command_function_ALUX(){

	lcd_lux_status;

	char charBuf[50];
	String str_lux;
	String str_cmd;
	int aNumber;
	char *arg;  

	arg = serial_cmd.next();

	if (arg != NULL) {
		aNumber = atoi(arg);
		lcd_lux_status=aNumber;

		// TODO: TEST
		lcd.setBacklight(lcd_lux_status);
	}

    str_lux = String(lcd_lux_status);
  	str_cmd = String("ALUX?" + str_lux);
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 
}


// Consulta version del software.
void ardufocuser_command_function_AVERS(){
	String str_cmd;
	char charBuf[50];
	str_cmd = String("AVER?1");
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 
}


/////////FUNCIONES PARA DEBUG ///////////////
void ardufocuser_command_function_ARUNA(){

	String str_cmd;
	char charBuf[50];
	str_cmd = String("ARUNA?");
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 
	motor.setMaxSpeed(500);
	motor.moveTo(30000);
}

void ardufocuser_command_function_ARUNB(){

	String str_cmd;
	char charBuf[50];
	str_cmd = String("ARUNB?");
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 
	motor.setMaxSpeed(500);
	motor.moveTo(-30000);
}

void ardufocuser_command_function_ASTOP(void){

	String str_cmd;
	char charBuf[50];
	str_cmd = String("ASTOP?");
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 
	motor.stop();
}

/*
void ardufocuser_command_function_ACALIBRATE(void){
}
*/

// Incializa parseador de comandos con sus correspondientes funciones asociadas.
void registerCommand(){

  serial_cmd.addCommand("AINIT", ardufocuser_command_function_AINIT);
  serial_cmd.addCommand("AMODE", ardufocuser_command_function_AMODE);
  serial_cmd.addCommand("ALCDPRINT", ardufocuser_command_function_ALCD_PRINT);
  serial_cmd.addCommand("AG", ardufocuser_command_function_AG);
  serial_cmd.addCommand("APOSITION", ardufocuser_command_function_APOSITION);
  serial_cmd.addCommand("ALTEMP", ardufocuser_command_function_ATEMP);
  serial_cmd.addCommand("AMICRO", ardufocuser_command_function_AMICRO);
  serial_cmd.addCommand("AFINE", ardufocuser_command_function_AFINE);
  serial_cmd.addCommand("ASPEED", ardufocuser_command_function_ASPEED);
  serial_cmd.addCommand("AR", ardufocuser_command_function_AR);
  serial_cmd.addCommand("AHLIMIT", ardufocuser_command_function_AHLIMIT);
  serial_cmd.addCommand("ASLIMIT", ardufocuser_command_function_ASLIMIT);
  serial_cmd.addCommand("ASILIMIT", ardufocuser_command_function_ASILIMIT);
  serial_cmd.addCommand("ASOLIMIT", ardufocuser_command_function_ASOLIMIT);
  serial_cmd.addCommand("AVERS", ardufocuser_command_function_AVERS);
  serial_cmd.addCommand("ARUNA", ardufocuser_command_function_ARUNA);
  serial_cmd.addCommand("ARUNB", ardufocuser_command_function_ARUNB);
  serial_cmd.addCommand("ASTOP", ardufocuser_command_function_ASTOP);
  serial_cmd.addCommand("ALUX", ardufocuser_command_function_ALUX);
  // serial_cmd.addCommand("ACALIBRATE", function_SETCOUNT);
}