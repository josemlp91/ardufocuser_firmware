//Definimos los indices de cada comando.
//OJO: Para añadir un comando debemos asegurarnos que lo añadimos en la posición que corresponde con su indice.

#define AINIT   				0    // Iniciar modo funcionamiento Ardufocuser.
#define AMODE   				1    // Estableceer modo de funcionamiento.
#define AG   		  			2    // Mover enfocador hasta posición pasada como dato.
#define APOSITION 			3    // Solicitar posición actual del enfocador.
#define ATEMP   				4    // Solicitar temperatura de la lente al enfocador.
#define ALTEMP   				5    // Solicitar temperatura de la lente al enfocador.
#define AMICRO   				6    // Establecer multiplicación del micropaso.
#define AFINE   				7    // Establecer pasos que da el enfocador por cada pulso.
#define ASPEED   				8    // Establecer la velocidad del movimiento del enfocador.
#define AACC   					9    // Establecer la aceleración del movimiento del enfocador.
#define AR   		  		 10    // Fijar posición reletiva del enfocador en un valor personalizado.
#define AHLIMIT   		 11    // Consultar si el enfocador ha llegado a un limite hardware.
#define	ASLIMIT				 12	   // Consultar si el enfocador ha llegado a un limite software.
#define ASILIMIT  		 13    // Establecer  limite software inware.
#define ASOLIMIT  		 14    // Establecer  limite software outware.
#define AVERS   			 15    // Consultar version del firmware.
#define AMOV   				 16    // Consulta si el enfocador esta moviendose.
#define ARUNA   	     17    // Comando para debug: Mueve el motor del enfocador en una dirreción.
#define ARUNB   			 18    // Comando para debug: Mueve el motor del enfocador en una dirreción.
#define ASTOP   			 19    // Comando para debug: Detiene motor del enfocador
#define ALUX   	       20    // Modifica la intesindad de la ilumincaicon de la pantalla.
#define ACALIBRATE   	 21    // Configura los limites software.


// Añadir el comando en la posición que correspondiente con el indice.
char *allcmdArdu[22]={
						"AINIT",
						"AMODE",
						"AG",
						"APOSITION",
						"ATEMP",
						"ALTEMP",
						"AMICRO",
						"AFINE",
						"ASPEED",
						"AACC",
						"AR",
						"AHLIMIT",
						"ASLIMIT",
						"ASILIMIT",
						"ASOLIMIT",
						"AVERS",
						"AMOV",
						"ARUNA",
						"ARUNB",
						"ASTOP",
						"ALUX",
						"ACALIBRATE"
};


// Definición de las funciones asociadas a los comando.
// Más info de la función junto al define correspondiente.

// Ajusta modo autofocus.
void ardufocuser_command_function_AINIT(){
	Serial.println("Modo ardufocus");
}

// Ajusta modo.
void ardufocuser_command_function_AMODE(){
	Serial.println("Ajustando modo");
}

// Mueve motor hasta la psocición pasada como parametro.
void ardufocuser_command_function_AG(){
	int Ipar=atoi(data);
    motor.moveTo(Ipar);
    if (DEBUG) { Serial.println(motor.currentPosition());  }
}

// Envia mensaje indicando posición.
void ardufocuser_command_function_APOSITION(){
	String posA(motor.currentPosition());
    sendMessageToIndi("APOSITION?"+posA);
}

// Envia mensaje informando de la temperatura del sensor.
void ardufocuser_command_function_ATEMP(){
	String atemp(temp_perform);
    sendMessageToIndi("ALTEMP?"+atemp);
}

// Envia mensaje informando de la temperatura del sensor.
void ardufocuser_command_function_ALTEMP(){
	String atemp(temp_perform);
    sendMessageToIndi("ALTEMP?"+atemp);
}

// Ajusta numero de paso en movimiento paso a paso.
void ardufocuser_command_function_AFINE(){
	int Ipf=atoi(data);
    if (Ipf>MAXSTEPPXPULSA) Ipf=MAXSTEPPXPULSA;
    if (Ipf<1) Ipf=1;

    stepPerPulse=Ipf;
    String pf(Ipf);
    hadToReadstepPerPulse=false;
    sendMessageToIndi("AFINE?"+pf);
}

// Ajusta la velocidad de movimiento del motor.
void ardufocuser_command_function_ASPEED(){
	int Ivel=atoi(data);
     if (Ivel>MAXVEL) Ivel=MAXVEL;
     if (Ivel<MINVEL) Ivel=MINVEL;
     speed=Ivel;
     String vel(Ivel);
     hadToReadspeed=false;
     sendMessageToIndi("ASPEED?"+vel);
}

// Ajusta la aceleración del motor.
void ardufocuser_command_function_AACC(){
	int Iacc=atoi(data);
     if (Iacc>MAXACC) Iacc=MAXACC;
     if (Iacc<MINACC) Iacc=MINACC;

     String acc(Iacc);
     motor.setAcceleration(Iacc);
     sendMessageToIndi("AACC?"+acc);
}

// Ajusta posición actual como posición pasada por argumento.
void ardufocuser_command_function_AR(){
	int Ir=atoi(data);
      String r(Ir);
      motor.setCurrentPosition(Ir);
      sendMessageToIndi("AR?"+r);
}

// Consulta si estamos en alguna posición final de recorrido.
void ardufocuser_command_function_AHLIMIT(){
	if (limitRunActiveA)sendMessageToIndi("AHLIMIT?I");
    else if (limitRunSoftwareB) sendMessageToIndi("AHLIMIT?O");
    else sendMessageToIndi("AHLIMIT?");
}

// Consulta si el motor se esta moviendo.
void ardufocuser_command_function_AMOV(){
	 // Consultar si el enfocador ha llegado a la posicion destino (si se esta moviendo).
    if (motor.distanceToGo() == 0) { sendMessageToIndi("ASTOP?");}
    else {sendMessageToIndi("ARUN?");}
}

// Set  high o low en pines correspondientes a ...............................................................................................................//(MS1, MS2, MS3)
void setMicroFullStep()     { digitalWrite(PINMICROSTEP_MS1,LOW);   digitalWrite(PINMICROSTEP_MS2,LOW);   digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=1; } //( 0,   0,   0 )
void setMicroHalfStep()     { digitalWrite(PINMICROSTEP_MS1,HIGH);  digitalWrite(PINMICROSTEP_MS2,LOW);   digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=2; } //( 1,   0,   0 )
void setMicroQuarterStep()  { digitalWrite(PINMICROSTEP_MS1,LOW);   digitalWrite(PINMICROSTEP_MS2,HIGH);  digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=3; } //( 0,   1,   0 )
void setMicroEighthStep()   { digitalWrite(PINMICROSTEP_MS1,HIGH);  digitalWrite(PINMICROSTEP_MS2,HIGH);  digitalWrite(PINMICROSTEP_MS3,LOW);  microStep=4; } //( 1,   1,   0 )
void setMicroSixteenthStep(){ digitalWrite(PINMICROSTEP_MS1,HIGH);  digitalWrite(PINMICROSTEP_MS2,HIGH);  digitalWrite(PINMICROSTEP_MS3,HIGH); microStep=5; } //( 1,   1,   1 )

// Realiza configuración de micropasos, diviendo cada paso en la mitad, cuarto, octava o dieciseisava.
void ardufocuser_command_function_AMICRO(){
	int Imicro=atoi(data);

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
   String smicro(microStep);
	sendMessageToIndi("AMICRO?"+ smicro );

}

// Consuta limistes software
void ardufocuser_command_function_ASLIMIT(){
	if (limitRunSoftwareActiveA || limitRunSoftwareActiveA){
		sendMessageToIndi("ASLIMIT?1");
	}
	else sendMessageToIndi("ASLIMIT?0");
}

// Ajusta limite software A.
void ardufocuser_command_function_ASILIMIT(){
	int Ilima=atoi(data);
	if  (Ilima < motor.currentPosition()){
     	limitRunSoftwareA=Ilima;
     }
	String slima(limitRunSoftwareA);
	sendMessageToIndi("ASILIMIT?"+ slima );
}

// Ajusta limite software B.
void ardufocuser_command_function_ASOLIMIT(){
	 int Ilimb=atoi(data);
	 if (Ilimb > motor.currentPosition()){
    	 limitRunSoftwareB=Ilimb;
 	}
 	 String slimb(limitRunSoftwareB);
	sendMessageToIndi("ASOLIMIT?"+ slimb );
}

// Modifica brillo de la pantalla LCD
void ardufocuser_command_function_ALUX(){
	  int Ilux=atoi(data);
	  BRIGHTNESS=Ilux;
	  setBrightness(BRIGHTNESS);
}

// Consulta version del software.
void ardufocuser_command_function_AVERS(){
	sendMessageToIndi("AVERS?1");
}

void ardufocuser_command_function_ACALIBRATE(){
}

/////////FUNCIONES PARA DEBUG ///////////////

void ardufocuser_command_function_ARUNA(){
	 motor.setMaxSpeed(500);
	 motor.moveTo(30000);
}

void ardufocuser_command_function_ARUNB(){
	motor.setMaxSpeed(500);
	 motor.moveTo(-30000);
}

void ardufocuser_command_function_ASTOP(){
	  motor.stop();
}
