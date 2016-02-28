//Definimos los indices de cada comando.
//OJO: Para añadir un comando debemos asegurarnos que lo añadimos en la posición que corresponde con su indice.

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

/*

String global_input_string = "";
String global_command = ""; 
String global_arg = "";  
boolean global_flag_string_is_complete = false;

// Evento serie, se ejecuta dado un evento serie.
void serialEvent() {

  // Delimitador entre comando y parametro.
  char cmd_arg_separator='?';
  int separator_index;	

  while (Serial.available()) {
    // Leemos caracter por caracter.
    char in_char = (char)Serial.read();
    global_input_string += in_char;
    
    if (in_char == '\n') {
      global_flag_string_is_complete = true;
      
      separator_index = global_input_string.indexOf(cmd_arg_separator);
      global_command = global_input_string.substring(0, separator_index);
      global_arg = global_input_string.substring(separator_index+1, global_input_string.length());
      

      lcd.clear();
      lcd.print(global_command);
      lcd.print(" ");
      lcd.print(global_arg);

    }
  }
}

*/



// Ajusta modo autofocus.
/*
int ardufocuser_command_function_AINIT(int argc, char** argv){
   char charBuf[50];
   
   mode = 1;
   
   String str_mode = String(mode);
   String str_cmd = String("AINIT " + str_mode );
   str_cmd.toCharArray(charBuf, 50); 
   
   shell_printf(charBuf);
   return SHELL_RET_SUCCESS;
}
*/

/*
 int ardufocuser_command_function_ALCD_PRINT(int argc, char** argv)
  {
    if (argc > 1){
      String str=argv[1];
      lcd.clear();
      lcd.print(str);
      return SHELL_RET_SUCCESS;
    }
  }
*/

/*
int ardufocuser_command_function_AMODE(int argc, char** argv){

	char charBuf[50];
	String str_mode;
	String str_cmd;

	if (argc > 0){
		str_mode = String(argv[1]);
		mode = str_mode.toInt();
		str_cmd = String("AMODE " + str_mode );
	   	str_cmd.toCharArray(charBuf, 50); 
		shell_printf(charBuf);
	}

	else {
	   str_mode = String(mode);
	   str_cmd = String("AMODE " + str_mode );
	   str_cmd.toCharArray(charBuf, 50); 
	   shell_printf(charBuf);
	}

	return SHELL_RET_SUCCESS;
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