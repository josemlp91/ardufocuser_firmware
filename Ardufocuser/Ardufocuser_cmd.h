//Definimos los indices de cada comando.
//OJO: Para añadir un comando debemos asegurarnos que lo añadimos en la posición que corresponde con su indice.


// Instancia de SerialCommand, tokenizador y parseador de puerto serie.
SerialCommand serial_cmd; 


#define AINIT   				0    // Iniciar modo funcionamiento Ardufocuser.
#define AMODE   				1    // Estableceer modo de funcionamiento.
#define AG   		  			2    // Mover enfocador hasta posición pasada como dato.
#define APOSITION 			    3    // Solicitar posición actual del enfocador.
#define ATEMP   				4    // Solicitar temperatura de la lente al enfocador.
#define ALTEMP   				5    // Solicitar temperatura de la lente al enfocador.
#define AMICRO   				6    // Establecer multiplicación del micropaso.
#define AFINE   				7    // Establecer pasos que da el enfocador por cada pulso.
#define ASPEED   				8    // Establecer la velocidad del movimiento del enfocador.
#define AACC   					9    // Establecer la aceleración del movimiento del enfocador.
#define AR   		  		   10    // Fijar posición reletiva del enfocador en un valor personalizado.
#define AHLIMIT   		       11    // Consultar si el enfocador ha llegado a un limite hardware.
#define	ASLIMIT				   12	   // Consultar si el enfocador ha llegado a un limite software.
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
	String str_cmd;
	char *arg;  

	arg = serial_cmd.next();

  if (arg != NULL) {
   
    lcd.clear();
    lcd.print(arg);
    
    str_cmd = String("ALCDPRINT");
	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 

  }
  else {
    Serial.println("No arguments");
  }
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
	str_mode = arg;

	str_cmd = String("AMODE?" + str_mode );
   	str_cmd.toCharArray(charBuf, 50); 
	Serial.println(str_cmd); 
  }

  else {
       str_mode = String(mode);
	   str_cmd = String("AMODE?" + str_mode );
	   str_cmd.toCharArray(charBuf, 50); 
	   Serial.println(str_cmd);
  }

}



/*
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

*/






/*
// Mueve motor hasta la psocición pasada como parametro.
int ardufocuser_command_function_AG(int argc, char** argv){
	char charBuf[50];
	String str_position;
	String str_cmd;

	if (argc > 0){
		str_position = String(argv[1]);
		position = str_position.toInt();
		str_cmd = String("AG " + str_position );
	   	str_cmd.toCharArray(charBuf, 50); 
		shell_printf(charBuf);
	}

	else {

	  	position=position
	   //str_position = String(position);
	   str_cmd = String("AG " + str_position );
	   str_cmd.toCharArray(charBuf, 50); 
	   shell_printf(charBuf);
	}

	return SHELL_RET_SUCCESS;
}
*/

/*

// Envia mensaje indicando posición.
void ardufocuser_command_function_APOSITION(void){
	shell_printf("HOLA");
	return SHELL_RET_SUCCESS;
}

// Envia mensaje informando de la temperatura del sensor.
void ardufocuser_command_function_ATEMP(void){
	shell_printf("HOLA");
  return SHELL_RET_SUCCESS;
}

// Envia mensaje informando de la temperatura del sensor.
void ardufocuser_command_function_ALTEMP(void){
	shell_printf("HOLA");
	return SHELL_RET_SUCCESS;
}

// Ajusta numero de paso en movimiento paso a paso.
void ardufocuser_command_function_AFINE(void){
	shell_printf("HOLA");
	return SHELL_RET_SUCCESS;
}

// Ajusta la velocidad de movimiento del motor.
void ardufocuser_command_function_ASPEED(void){
	shell_printf("HOLA");
  return SHELL_RET_SUCCESS;
}

// Ajusta la aceleración del motor.
void ardufocuser_command_function_AACC(void){
	shell_printf("HOLA");
	return SHELL_RET_SUCCESS;
}

// Ajusta posición actual como posición pasada por argumento.
void ardufocuser_command_function_AR(void){
	shell_printf("HOLA");
  return SHELL_RET_SUCCESS;
}

// Consulta si estamos en alguna posición final de recorrido.
void ardufocuser_command_function_AHLIMIT(void){
	shell_printf("HOLA");
	return SHELL_RET_SUCCESS;
}

// Consulta si el motor se esta moviendo.
void ardufocuser_command_function_AMOV(void){
	 // Consultar si el enfocador ha llegado a la posicion destino (si se esta moviendo).
	 shell_printf("HOLA");
	 return SHELL_RET_SUCCESS;
}
*/

// Set  high o low en pines correspondientes a ...............................................................................................................//(MS1, MS2, MS3)
/*
void setMicroFullStep()     { digitalWrite(PINMICROSTEP_MS1,LOW);   digitalWrite(PINMICROSTEP_MS2,LOW);   digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=1; } //( 0,   0,   0 )
void setMicroHalfStep()     { digitalWrite(PINMICROSTEP_MS1,HIGH);  digitalWrite(PINMICROSTEP_MS2,LOW);   digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=2; } //( 1,   0,   0 )
void setMicroQuarterStep()  { digitalWrite(PINMICROSTEP_MS1,LOW);   digitalWrite(PINMICROSTEP_MS2,HIGH);  digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=3; } //( 0,   1,   0 )
void setMicroEighthStep()   { digitalWrite(PINMICROSTEP_MS1,HIGH);  digitalWrite(PINMICROSTEP_MS2,HIGH);  digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=4; } //( 1,   1,   0 )
void setMicroSixteenthStep(){ digitalWrite(PINMICROSTEP_MS1,HIGH);  digitalWrite(PINMICROSTEP_MS2,HIGH);  digitalWrite(PINMICROSTEP_MS3,HIGH); microStep=5; } //( 1,   1,   1 )
*/
// Realiza configuración de micropasos, diviendo cada paso en la mitad, cuarto, octava o dieciseisava.

/*
void ardufocuser_command_function_AMICRO(void){
	shell_printf("HOLA");
	return SHELL_RET_SUCCESS;

}

// Consuta limistes software
void ardufocuser_command_function_ASLIMIT(void){
	shell_printf("HOLA");
	return SHELL_RET_SUCCESS;
}

// Ajusta limite software A.
void ardufocuser_command_function_ASILIMIT(void){
	shell_printf("HOLA");
	return SHELL_RET_SUCCESS;
}

// Ajusta limite software B.
void ardufocuser_command_function_ASOLIMIT(void){
	shell_printf("HOLA");
	return SHELL_RET_SUCCESS;
}

// Modifica brillo de la pantalla LCD
void ardufocuser_command_function_ALUX(void){
	shell_printf("HOLA");
	return SHELL_RET_SUCCESS;
}

// Consulta version del software.
void ardufocuser_command_function_AVERS(void){
	shell_printf("HOLA");
  return SHELL_RET_SUCCESS;
}

void ardufocuser_command_function_ACALIBRATE(void){
}

/////////FUNCIONES PARA DEBUG ///////////////
void ardufocuser_command_function_ARUNA(void){
		shell_printf("HOLA");
    return SHELL_RET_SUCCESS;
}

void ardufocuser_command_function_ARUNB(void){
	shell_printf("HOLA");
  return SHELL_RET_SUCCESS;
}

void ardufocuser_command_function_ASTOP(void){
	shell_printf("HOLA");
	return SHELL_RET_SUCCESS;
}

*/

//////////////////////////////////////////////////////////////////////////////
// Funcion de escucha.



/*
int shell_reader(char * data)
{
  if (Serial.available()) {
    *data = Serial.read();
    return 1;
  }
  return 0;
}


void shell_writer(char data)
{
  Serial.write(data);
}


// Incializa parseador de comandos con sus correspondientes funciones asociadas.
void registerCommand(){
  // Configuramos función de escucha y de escritura.
  shell_init(shell_reader, shell_writer, 0);
  // Registramos todas los comandos con sus correspondientes funciones.
  shell_register(ardufocuser_command_function_AINIT, "AINIT");
  shell_register(ardufocuser_command_function_ALCD_PRINT, "ALCD_PRINT");
  shell_register(ardufocuser_command_function_AMODE, "AMODE");
  shell_register(ardufocuser_command_function_AG, "AG");



  //..
}
*/



// Incializa parseador de comandos con sus correspondientes funciones asociadas.
void registerCommand(){

  serial_cmd.addCommand("AINIT", ardufocuser_command_function_AINIT);
  serial_cmd.addCommand("AMODE", ardufocuser_command_function_AMODE);
  serial_cmd.addCommand("ALCDPRINT", ardufocuser_command_function_ALCD_PRINT);
  // serial_cmd.addCommand("AG", function_SETCOUNT);
  // serial_cmd.addCommand("APOSITION", function_GETCOUNT);
  // serial_cmd.addCommand("ALTEMP", function_SETCOUNT);
  // serial_cmd.addCommand("AMICRO", function_SETCOUNT);
  // serial_cmd.addCommand("AFINE", function_GETCOUNT);
  // serial_cmd.addCommand("ASPEED", function_SETCOUNT);
  // serial_cmd.addCommand("AR", function_SETCOUNT);
  // serial_cmd.addCommand("AHLIMIT", function_GETCOUNT);
  // serial_cmd.addCommand("ASLIMIT", function_SETCOUNT);
  // serial_cmd.addCommand("ASILIMIT", function_SETCOUNT);
  // serial_cmd.addCommand("ASOLIMIT", function_GETCOUNT);
  // serial_cmd.addCommand("AVERS", function_SETCOUNT);
  // serial_cmd.addCommand("AMOV", function_SETCOUNT);
  // serial_cmd.addCommand("ARUNA", function_GETCOUNT);
  // serial_cmd.addCommand("ARUNB", function_SETCOUNT);
  // serial_cmd.addCommand("ASTOP", function_SETCOUNT);
  // serial_cmd.addCommand("ALUX", function_SETCOUNT);
  // serial_cmd.addCommand("ACALIBRATE", function_SETCOUNT);


}