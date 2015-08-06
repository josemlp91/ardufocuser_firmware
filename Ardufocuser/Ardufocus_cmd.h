//Definimos los indices de cada comando.
//OJO: Para añadir un comando debemos asegurarnos que lo añadimos en la posición que corresponde con su indice.

#define AINIT   	0    // Iniciar modo funcionamiento Ardufocuser.
#define AMODE   	1    // Estableceer modo de funcionamiento.
#define AG   		2    // Mover enfocador hasta posición pasada como dato.
#define APOSITION   3    // Solicitar posición actual del enfocador.
#define ATEMP   	4    // Solicitar temperatura de la lente al enfocador.
#define ALTEMP   	5    // Solicitar temperatura de la lente al enfocador.
#define AMICRO   	6    // Establecer multiplicación del micropaso.
#define AFINE   	7    // Establecer pasos que da el enfocador por cada pulso.
#define ASPEED   	8    // Establecer la velocidad del movimiento del enfocador.
#define AACC   		9    // Establecer la aceleración del movimiento del enfocador.
#define AR   		10   // Fijar posición reletiva del enfocador en un valor personalizado.
#define AHLIMIT    	11   // Consultar si el enfocador ha llegado a un limite hardware.
#define	ASLIMIT		12	 // Consultar si el enfocador ha llegado a un limite software.
#define ASILIMIT   	13   // Establecer  limite software inware.
#define ASOLIMIT   	14   // Establecer  limite software outware.
#define AVERS   	15   // Consultar version del firmware.
#define AMOV   		16   // Consulta si el enfocador esta moviendose.
#define ARUNA   	17   // Comando para debug: Mueve el motor del enfocador en una dirreción.
#define ARUNB   	18   // Comando para debug: Mueve el motor del enfocador en una dirreción.
#define ASTOP   	19   // Comando para debug: Detiene motor del enfocador


char *allcmdArdu[20]={
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
						"ASTOP"
};


// Definición de las funciones asociadas a los comando.
// Más info de la función junto al define correspondiente.


void ardufocuser_command_function_AINIT(){
	Serial.println("Modo ardufocus"); 
}

void ardufocuser_command_function_AMODE(){
	Serial.println("Ajustando modo");  
}


void ardufocuser_command_function_AG(){
	int Ipar=atoi(data);
    motor.moveTo(Ipar); 
    if (DEBUG) { Serial.println(motor.currentPosition());  } 
}

void ardufocuser_command_function_APOSITION(){
	String posA(motor.currentPosition());
    sendMessageToIndi("APOSITION?"+posA);  
}

void ardufocuser_command_function_ATEMP(){
	String atemp(temp_perform);
    sendMessageToIndi("ALTEMP?"+atemp);   
}

void ardufocuser_command_function_ALTEMP(){
	String atemp(temp_perform);
    sendMessageToIndi("ALTEMP?"+atemp);   
}

void ardufocuser_command_function_AFINE(){
	int Ipf=atoi(data);
    if (Ipf>MAXSTEPPXPULSA) Ipf=MAXSTEPPXPULSA;
    if (Ipf<1) Ipf=1;

    stepPerPulse=Ipf;
    String pf(Ipf);
    hadToReadstepPerPulse=false;
    sendMessageToIndi("AFINE?"+pf); 
}

void ardufocuser_command_function_ASPEED(){
	int Ivel=atoi(data);
     if (Ivel>MAXVEL) Ivel=MAXVEL;
     if (Ivel<MINVEL) Ivel=MINVEL;
     speed=Ivel;
     String vel(Ivel);
     hadToReadspeed=false;
     sendMessageToIndi("ASPEED?"+vel);
}


void ardufocuser_command_function_AACC(){
	int Iacc=atoi(data);
     if (Iacc>MAXACC) Iacc=MAXACC;
     if (Iacc<MINACC) Iacc=MINACC;
     
     String acc(Iacc);  
     motor.setAcceleration(Iacc);
     sendMessageToIndi("AACC?"+acc);
}

void ardufocuser_command_function_AR(){
	int Ir=atoi(data);
      String r(Ir); 
      motor.setCurrentPosition(Ir);
      sendMessageToIndi("AR?"+r);
}

void ardufocuser_command_function_AHLIMIT(){
	if (limitRunActiveA)sendMessageToIndi("AHLIMIT?I");
    else if (limitRunSoftwareB) sendMessageToIndi("AHLIMIT?O"); 
    else sendMessageToIndi("AHLIMIT?"); 
}

void ardufocuser_command_function_AMOV(){
	 // Consultar si el enfocador ha llegado a la posicion destino (si se esta moviendo).
    if (motor.distanceToGo() == 0) { sendMessageToIndi("ASTOP?");} 
    else {sendMessageToIndi("ARUN?");}
}


void ardufocuser_command_function_AMICRO(){
	Serial.println("Ajusta micropasos");
}


void ardufocuser_command_function_ASLIMIT(){
	Serial.println("Consultar si el enfocador ha llegado a un limite software.");
}

void ardufocuser_command_function_ASILIMIT(){
	Serial.println("Establecer limite software inware.");
}

void ardufocuser_command_function_ASOLIMIT(){
	Serial.println("Establecer limite software outware. ");
}

void ardufocuser_command_function_AVERS(){
	sendMessageToIndi("AVERS?1"); 
}


void ardufocuser_command_function_ARUNA(){
	 motor.moveTo(30000);
}

void ardufocuser_command_function_ARUNB(){
	 motor.moveTo(-30000);
}

void ardufocuser_command_function_ASTOP(){
	  motor.stop();
}
