
#define DEBUG 0

#include <TimerOne.h>
//#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include "Ardufocus_config.h"
#include <Wire.h>
#include <math.h>
#include <nunchuck.h>
#include <EEPROM.h>


// Iniciamos nunchuck
WiiChuck chuck = WiiChuck();

// Configuramos pines POLOLU
AccelStepper motor(1, PINSTEP, PINDIR);

// Configuramos LCD I2C en canal 27.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variable para boton activo en cada momento.
int button=btnNONE;

// Brillo por defecto.
int BRIGHTNESS = 50;

// Variables para controlar los limites findes de carrera hardware.
bool limitRunActiveA=false; bool limitRunActiveB=false;

// Variables para controlar los limites fines de carrera software.
bool limitRunSoftwareActiveA=false; bool limitRunSoftwareActiveB=false;
int  limitRunSoftwareA=-300; int  limitRunSoftwareB=300;

// Variable para controlar velocidad.
int speed = 0;

// Variable para controlar pasos por cada pulso.
int stepPerPulse = 0;

// Variable que controla la posición del motor en cada
int position;

// Variables para controlar la temperatura.
int temp=0; int conT=0; float temp_average=0; int temp_perform;   //Temperature in the moment of focus change.

// Varaibles para controlar estados pasados del sistema.
int lastTimeUpdate=1000;
int lastPulse=btnNONE;
int lastPulse2=btnNONE;
int lastspeed = 0;
int laststepPerPulse = 0;
int lastposition;
int lastTemp[10];
int microStep=1;
bool motorIsRun;

bool hadToReadspeed=true;
bool hadToReadstepPerPulse=true;
long lastTimeReadButton = 0;
long lastTimeReadController = 0;
//WorkMode mode;

// Tamaño de los buffer de lectura.
const int bSize = 20;
const int lenghtcommand=10;
const int lenghtdata=15;

// Parametros para controlar lectura serie.
int byteCount;
char buffer[bSize];     // buffer
char command[bSize];    // almacenamos comando
char data[bSize];       // almacenamos parametro


#include "Ardufocus_cmd.h"

/*
* Funcion que lee los botones.
* return boton
*/
int read_LCD_buttons(int adc_key){

//Hacer que estos valores se configuren con una rutina  automatica ...
 if (adc_key > 1000) return btnNONE;
 if (adc_key < 50)   return btnRIGHT;
 if (adc_key < 250)  return btnUP;
 if (adc_key < 450)  return btnDOWN;
 if (adc_key < 650)  return btnLEFT;
 if (adc_key < 850)  return btnSELECT;
 return btnNONE;

}

/*
* Funcion que imprime en LCD un saludo inicial.
*/
void welcome(String msgGreet){

  //lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print(msgGreet);
  delay(3000);
  lcd.setCursor(0,0);
  lcd.print("                ");

}

/*
* Funcion que ajusta el brillo de la LCD.
*/
void setBrightness(int bright){ lcd.backlight(); }  //Brillo del LCD
void noBrightness(int bright) { lcd.noBacklight(); }  //Apaga el brillo del LCD

/*
* Rutinas que se ejecutan bajo la interrupción hardware
* asociada a su corespondiente fin de carrera.
*/
void finA(){ limitRunActiveA=true; }
void finB(){ limitRunActiveB=true; }

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
}

/*
* Rutinas que actualiza los controles manuale, cuando pulsamos algun boton o actuamos sobre algun potenticometro.
*/
void readManualController(){

  //int button2;
  // Para evitar sobrecargar la placa hacemos las lecturas dado un intervalo de tiempo.
  // Si ya se a cumplido el intervalo de tiempo, realizamos lectura.

  /* Este vloque se usaba con unos botones analogicos.
  if (millis() > lastTimeReadButton + 5) {
    int adc_key = analogRead(PIN_BUTTON);

     Pare evitar rebotes estraños en la lectura de los botones,
    reliazamos dos lecturas, separadas por un delay

    button = read_LCD_buttons(adc_key);
    delayMicroseconds(200);   // Este delay debe ser lo suficiente para evitar el rebote y que mantenga la interactividad del control.
    button2 = read_LCD_buttons(adc_key);

    if (button!=button2){button=btnNONE;}

    // Refrescamos momento de actualización.
    lastTimeReadButton = millis();
  }

  // Si no se cumple intervalo de tiempo, utilizamos el valor anterior.
  else {
    button=lastPulse;
  }
  */

  // Lectura de los potenciometros.
  if (millis() > lastTimeReadController + 200) {

     int s = analogRead(PIN_POTA);
     // Cuando bajoamos el potenciometro al valor 0 forzamos a actualizar la velocidad.
     // Esto se hace así para contemplar la posibilidad de que se esten modificando los valores de forma remota.
     if (s==0) hadToReadspeed=true;
     if (hadToReadspeed){
        speed = map(s, 0, 1024, MINVEL, MAXVEL );
     }

     // Cuando bajoamos el potenciometro al valor 0 forzamos a actualizar los pasos por pulso.
     int sS = analogRead(PIN_POTB);
     if (sS==0) hadToReadstepPerPulse=true;

     if (hadToReadstepPerPulse){
        stepPerPulse = map(sS, 0, 1024, 1, MAXSTEPPXPULSA);
     }

     if (abs(motor.speed() - speed ) > 2) {
       motor.setMaxSpeed(speed);
     }

     // Refrescamos momento de actualización.
     lastTimeReadController = millis();
    }
}


/*
* Lee otros sensores o perifericos.v
*/
void readOtherSensor(){
 // Other Sensor
 //limitRunActiveA=digitalRead(PIN_LIMITRUNA);
 //limitRunActiveB=digitalRead(PIN_LIMITRUNB);
 //limitRunActiveA=limitRunActiveB=false;

}


/*
* Lee la temperatura.
*/
long lastTimeReadTemp = 0;
float readTemperature(){

  if (millis() > lastTimeReadTemp + 10000) {
      float t;
      t=analogRead(PIN_TEMSENSOR)* 0.48828125;
      if (abs(t-temp_average)<=2) temp_average=t;

      lastTimeReadTemp = millis();
    }

  return temp_average;

}

/*
* Actualiza mensajes que se muestran por pantalla LCD.
*/
long lastTimeUpdateLcd = 0;
void updateLCD(){


  // Si cumple el tiempo minimo de refresco.
  if (millis() > lastTimeUpdateLcd + 15) {

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

      // Actualizamos boton anterior al boton actual.
      lastPulse=button;

      }
    }

    if (millis() > lastTimeUpdateLcd + 200) {

       // Actualizamos zona de la temperatura
       lcd.setCursor(0,0);
       lcd.print("TEMP:   ");
       lcd.setCursor(5,0);
       lcd.print((int)temp_average);

       // si se produce un cambio significativo en el control de la velocidad.
       //if ((lastspeed>speed+1) or (lastspeed<speed-1))  {
       // Actualizar zona LCD donde se muestra la velocidad.
         lcd.setCursor(8,1);
         lcd.print("SP:    ");
         lcd.setCursor(12,1);
         lcd.print(speed);
         lastspeed=speed;
       //}

       // Si se produce un cambio significativo en el control de los pasos por pulso.
       //if ((laststepPerPulse>stepPerPulse+1) or (laststepPerPulse<stepPerPulse-1))  {
       // Actualizar zona LCD donde se muestran los pasos por pulso.

         lcd.setCursor(0,1);
         lcd.print("ST:   ");
         lcd.setCursor(3,1);
         lcd.print(stepPerPulse);
         laststepPerPulse=stepPerPulse;
      //}

       // Si se produce un cambio significativo en la posicion.
       //if ((lastposition>position+1) or (lastposition<position-1)){
       // Actualizar zona LCD donde se muestran la posición.
         lcd.setCursor(8,0);
         lcd.print("POS:    ");
         lcd.setCursor(12,0);
         lcd.print(position);
         lastposition=position;
     //}


     // Refrescamos momento de actualizacion.
     lastTimeUpdateLcd = millis();
  }
  }

}


// long lastTimeReadTemp = 0;
// void readTemperature(){

//   if (millis() > lastTimeReadTemp + 100) {

//     lastTemp[conT]= analogRead(PIN_TEMSENSOR) ;
//     conT++;
//     if (conT==10){
//       Serial.println(conT);
//       Serial.println(temp_average);
//       for (int i = 0; i<10; ++i){
//         temp_average+=lastTemp[i];


//       }
//       temp_average/=10;
//       temp_average*=0.48828125;
//       conT=0;
//     }

//     lastTimeReadTemp = millis();
//   }

// }





bool changeThermalOptical(int temp_average, int temp_perform){
   if ((temp_average>temp_perform+MAX_CHANGE_TEMP) || (temp_average<temp_perform-MAX_CHANGE_TEMP)) return true;
   else return false;
}

void enable_warning() { digitalWrite(PINLED_WARNING,HIGH); }
void disable_warning(){ digitalWrite(PINLED_WARNING,LOW); }

void manualPerformance(){

  //if (mode!=REMOTE){
  //motor.setAcceleration(DEF_ACC);   // Esta funcion no se puede ejecutar mucho
  position=motor.currentPosition();

  if (changeThermalOptical(temp_average, temp_perform)) enable_warning();

  switch (button){

    case btnNONE: {

                    if ((lastPulse==btnLEFT)||(lastPulse==btnRIGHT)){
                      motor.stop();
                    }
                   lastPulse=btnNONE;

                   break;

    }

    case btnRIGHT: {

                    motor.moveTo(motor.currentPosition()-3000);
                    lastPulse=btnRIGHT;

                    break;
    }

    case btnLEFT:   {
                      motor.moveTo(motor.currentPosition()+3000);
                      lastPulse=btnLEFT;

                      break;
    }

    case btnUP:   {
                    if (lastPulse!=btnUP){
                        motor.moveTo(motor.currentPosition() + stepPerPulse);
                    }

                    lastPulse=btnUP;

                    break;
    }

    case btnDOWN: {
                    if (lastPulse!=btnDOWN){
                        motor.moveTo(motor.currentPosition() - stepPerPulse);
                    }

                    lastPulse=btnDOWN;

                    break;
    }

    case btnSELECT: {
                      temp_perform=temp_average;
                      disable_warning();
                      motor.setCurrentPosition(0);

                      break;
    }

  }

  button=btnNONE;
}


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

  //if (mode!=MANUAL){
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

  //}

 }

/*
* Enviamos comando por pueto serie, destinado a ser leido por el servidor Indi.
*/
void sendMessageToIndi(String message){

  int len=message.length();
  for (int i=len;i<bSize;i++){
    message+=' ';
  }

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
  else if (!(strcmp (command,allcmdArdu[ARUNA])) )     { ardufocuser_command_function_ARUNA();     }
  else if (!(strcmp (command,allcmdArdu[ARUNB])))      { ardufocuser_command_function_ARUNB();     }
  else if (!(strcmp (command,allcmdArdu[ASTOP])))      { ardufocuser_command_function_ASTOP();      }
  else if (!(strcmp (command,allcmdArdu[ALUX])))      { ardufocuser_command_function_ALUX();      }

}


long lastCurrentPos= millis();

void sendCurrentposition(){

  if (millis()> lastCurrentPos + 1000){
    String r(motor.currentPosition());
    sendMessageToIndi("APOSITION?"+ r);

    if (motor.distanceToGo() == 0) { sendMessageToIndi("ASTOP?"); }
  lastCurrentPos=millis();
  }

}

/*
 * Controlador WiiNunckuck para Ardufocuser
 */
void nunckuckController(){

  chuck.update();
  if (chuck.rightJoy()){
    if (chuck.zPressed()){
      motor.moveTo(limitRunSoftwareB);
    }

    else if (lastPulse2!=btnDOWN){
          motor.moveTo(motor.currentPosition() + stepPerPulse);
    }
      lastPulse2=btnDOWN;
  }

   else if  (chuck.leftJoy()){
     if (chuck.zPressed()){
        motor.moveTo(limitRunSoftwareA);
      }

      else if (lastPulse2!=btnUP){
        motor.moveTo(motor.currentPosition() - stepPerPulse);
      }
        lastPulse2=btnUP;
   }

   else  {
    lastPulse2=btnNONE;
    motor.moveTo(motor.currentPosition());
   // motor.stop();

   }

}

/// Ver libreria: http://playground.arduino.cc/Code/DatabaseLibrary
/// Por si llega a ser útil.
//////////ALMACENAR CONFIGURACION EN EEPROM.
int eeprom_load_integer(int base_direction){
  byte L;
  byte H;
  int data;

  H = EEPROM.read(base_direction);
  L = EEPROM.read(base_direction +1);

  data = L | H << 8;
  return data;
}

void eeprom_save_integer(int base_direction, int data){

  byte L;
  byte H;

  H = highByte(data);
  L = lowByte(data);

  EEPROM.write(base_direction, H);
  EEPROM.write(base_direction+1, L);
}

int last_eeprom_time_try_tosave=0;
void eeprom_save_position(){

  byte position_L;
  byte position_H;
  int eepron_position;

  if (millis() > last_eeprom_time_try_tosave + 2000) {
    eepron_position=eeprom_load_integer(EEPROM_DIR_POSITION);

    if (eepron_position!=position){
      eeprom_save_integer(EEPROM_DIR_POSITION,position)
      last_eeprom_time_try_tosave = millis();
    }
  }
}

void setup() {

  // Iniciamos nunckuck
  chuck.begin();
  chuck.update();

  // Iniciamos comunicación serie.
  Serial.begin(9600);

  // Habilitamos salidas para seleccionar micropasos.
  pinMode(PINMICROSTEP_MS1, OUTPUT);
  pinMode(PINMICROSTEP_MS2, OUTPUT);
  pinMode(PINMICROSTEP_MS3, OUTPUT);

  //Configuracion pines entrada salida.
  pinMode(PINLED_WARNING, OUTPUT);

  // Velocidad y aceletación por defecto.
  motor.setMaxSpeed(200);
  motor.setAcceleration(1000);

  //Variables auxiliares para  saber cuando actualizar la LCD
  lastTimeUpdate=millis();
  lastspeed=speed;
  laststepPerPulse = stepPerPulse;
  lastposition= position;
  temp_average=analogRead(PIN_TEMSENSOR)* 0.48828125;

  //Interrupciones software y hardware.
  Timer1.initialize(50);
  Timer1.attachInterrupt( timerFunction );
//         attachInterrupt ( 0, finA,RISING);
//         attachInterrupt ( 1, finB,RISING);

  //Ajustamos el brillo y saludo inicial.
   setBrightness(BRIGHTNESS);
   welcome("  ARDUFOCUSER");

}




void loop(){

  readManualController();
  readOtherSensor();
  //readTemperature();
  updateLCD();
  //manualPerformance();
  sendCurrentposition();
  remoteManager();
  //nunckuckController();
}
