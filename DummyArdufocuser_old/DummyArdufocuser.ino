#define DEBUG 0

// DEFAULT VALUES.
#define MAXSTEPPXPULSA   101
#define MAXVEL   501
#define MINVEL   1
#define MAXACC   0
#define MINACC   501
#define DEF_ACC  300

int BRIGHTNESS = 50;

// Variables para controlar los limites findes de carrera hardware.
bool limitRunActiveA=false; bool limitRunActiveB=false;

// Variables para controlar los limites fines de carrera software.
bool limitRunSoftwareActiveA=false; bool limitRunSoftwareActiveB=false;
int  limitRunSoftwareA=-300; int limitRunSoftwareB=300;

// Variable para controlar velocidad.
int speed = 0;

// Variable para controlar pasos por cada pulso.
int stepPerPulse = 0;

// Variable que controla la posición del motor en cada
int position=0;

// Tamaño de los buffer de lectura.
const int bSize = 20;
const int lenghtcommand=10;
const int lenghtdata=15;

// Parametros para controlar lectura serie.
int byteCount;
char buffer[bSize];     // buffer
char command[bSize];    // almacenamos comando
char data[bSize];       // almacenamos parametro


#include "DummyArdufocus_cmd.h"

/*
* Parseer del comando que se comunica por el puerto serie,
* el formato siempre es CMD?DATA    <comando + delimitador + datos>
*
* Pasamos como parametro el delimitador.
*/
void serialParser(char delimiter) {

 if (Serial.available()){
   byteCount = -1;
   // Leemos puerto serie hasta completar buffer.
   for (int x=0;x<bSize;x++){
      buffer[x]=Serial.read();
      byteCount++;
   }
  }

    // Si hemos leido algun dato.
    if (byteCount  > 0) {
        char c;
        int i=0;
        // Volcamos contenido del buffer en el contenedor del comando, buscando el delimitador.
        for (i; ((i<byteCount) and (c!= delimiter)) ; i++){
              c=buffer[i];
              if (c!= delimiter) command[i]=c;

        }

        int a=0;
         // Volcamos contenido del buffer en el contenedor del dato.
        for (i; (i<byteCount) ; i++){
          c=buffer[i];

          if ( c>47 && c < 123) data[a]=c;
          a++;
      }
    }

   //Limpiamos el buffer
   memset(buffer, 0, bSize);
}


void confirmPositionGet(int pos){}


// Dada la lectura serie, carga el comando y el dato.
void remoteManager(){

    char  d='?';
    serialParser(d);

    if (byteCount  > 0) {

      if      (command[0]=='F'){ /*Implementar*/ }           // Si detectamos comando del repertorio de instrucciones del Robofocus.
      else if (command[0]=='A'){  driveCommandArdufocus(); } // Si detectamos comando del repertorio de instrucciones del Ardufocuser.
      else    Serial.println("No comando");                  // No pertenece a ningun repertorio de instrucciones.

    //En modo debug: Hacemos eco de los comandos que recibe para depurar que se estan tratando correctamente.
    if (DEBUG){
      Serial.print("CMD      : ");
      Serial.println(command);
      Serial.print("PAR      : ");
      Serial.println(data);
      Serial.flush();
    }
  }

  //Limpiamos contenedor del comando.
  memset(command, 0, sizeof(command));
  //Limpiamos contenedor de los datos.
  memset(data, 0, sizeof(data));
  // Reseteamos contador de bytes leidos.
  byteCount=0;



 }

/*
* Enviamos comando por pueto serie, destinado a ser leido por el servidor Indi.
*/
void sendMessageToIndi(String message){
  int len=message.length();
  for (int i=len;i<bSize;i++){ message+=' ';}
  Serial.println(message);
  Serial.flush();
}

/*
* Maneja comandos Ardofocuser.
* Dado el comando y el dato, actua sobre el motor, lcd, sensores o controles segun proceda.
*/
void driveCommandArdufocus(){

  if      (!(strcmp(command,allcmdArdu[AINIT])) )      { ardufocuser_command_function_AINIT();     }
  else if (!(strcmp (command,allcmdArdu[AMODE])))      { ardufocuser_command_function_AMODE();     }
  else if (!(strcmp (command,allcmdArdu[AG])))         { ardufocuser_command_function_AG();        }
  else if (!(strcmp (command,allcmdArdu[APOSITION])))  { ardufocuser_command_function_APOSITION(); }
  else if (!(strcmp (command,allcmdArdu[ATEMP])))      { ardufocuser_command_function_ATEMP();     }
  else if (!(strcmp (command,allcmdArdu[ALTEMP])))     { ardufocuser_command_function_ALTEMP();    }
  else if (!(strcmp (command,allcmdArdu[AFINE])))      { ardufocuser_command_function_AFINE();     }
  else if (!(strcmp (command,allcmdArdu[ASPEED])))     { ardufocuser_command_function_ASPEED();    }
  else if (!(strcmp (command,allcmdArdu[AACC])))       { ardufocuser_command_function_AACC();      }
  else if (!(strcmp (command,allcmdArdu[AR])))         { ardufocuser_command_function_AR();        }
  else if (!(strcmp (command,allcmdArdu[AHLIMIT])))    { ardufocuser_command_function_AHLIMIT();   }
  else if (!(strcmp (command,allcmdArdu[AMICRO])) )    { ardufocuser_command_function_AMICRO();    }
  else if (!(strcmp (command,allcmdArdu[ASLIMIT] )) )  { ardufocuser_command_function_ASLIMIT();   }
  else if (!(strcmp (command,allcmdArdu[ASILIMIT])) )  { ardufocuser_command_function_ASILIMIT();  }
  else if (!(strcmp (command,allcmdArdu[ASOLIMIT])))   { ardufocuser_command_function_ASOLIMIT();  }
  else if (!(strcmp (command,allcmdArdu[AVERS]))   )   { ardufocuser_command_function_AVERS();     }
  else if (!(strcmp (command,allcmdArdu[AMOV]))  )     { ardufocuser_command_function_AMOV();      }
  else if (!(strcmp (command,allcmdArdu[ALUX])))       { ardufocuser_command_function_ALUX();      }

}


long lastCurrentPos= millis();
void sendCurrentposition(){

  if (millis()> lastCurrentPos + 1000){
    String pp(position);
    sendMessageToIndi("APOSITION?"+pp);
    lastCurrentPos=millis();
  }

}


void setup() {
  Serial.begin(9600);
  position=100;
}


void loop(){
  sendCurrentposition();
  remoteManager();
}
