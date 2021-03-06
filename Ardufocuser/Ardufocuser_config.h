
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


// DEFAULT VALUES.

// Máximo valor para el control de pasos por pulso.
#define MAXSTEPPXPULSA   51

// Máximo valor para el control de la velocidad de giro del motor.
#define MAXVEL   500

// Minimo valor para la velocidad de giro del motor.
#define MINVEL   1

// Máximo valor para la aceleración de giro del motor.
#define MAXACC   500

// Máximo valor para la aceleración de giro del motor.
#define MINACC   0

// Aceleración por defecto de giro del motor.
#define DEF_ACC  300


/// PINEOUT MAP. 
// Pines de control del motor.
#define PINDIR 9
#define PINSTEP 10

// Pines de control de bumper. 
// Ojo: deben corresponder con los pines con interrupciones hardware.
#define PIN_LIMITRUNA 2
#define PIN_LIMITRUNB 3 

// Pin LED aviso.
#define PINLED_WARNING 50
#define MAX_CHANGE_TEMP 2

// Pines de configuración de los micropasos de motor.
#define PINMICROSTEP_MS1 11
#define PINMICROSTEP_MS2 12
#define PINMICROSTEP_MS3 13

// Direccion de escritura en memoria  eeprom
# define EEPROM_SESSION_ADDRESS 20  // Es un  valor arbitrario, (Arduino UNO 512 bytes de EEPROM)

// analogic
// I2C
#define PIN_TEMSENSOR A2
#define PIN_POTA A0
#define PIN_POTB A1

// Se asume dado la arquitectura de la placa.
#define PIN_I2C_SDA A4
#define PIN_I2C_SCL A5


// Filas y Columnas de la pantalla LCD
# define LCD_ROWS 2
# define LCD_COLUMNS 16

// Constantes para los modos de funcionamiento.
# define ARDUFOCUS_MODE 1
# define ROBOFOCUS_MODE 2
# define ONLY_MANUAL 3
# define OTHER_MODE 4


const int btnRIGHT=0;
const int btnUP=1;
const int btnDOWN=2;
const int btnLEFT=3;
const int btnSELECT=4;
const int btnNONE=5;

