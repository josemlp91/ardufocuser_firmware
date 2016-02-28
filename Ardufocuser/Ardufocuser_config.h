
// DEFAULT VALUES.


// Máximo valor para el control de pasos por pulso.
#define MAXSTEPPXPULSA   100

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
# define EEPROM_DIR_POSITION 20
# define EEPROM_DIR_SPEED 30
# define EEPROM_DIR_STEEP 40
# define EEPROM_DIR_MICRO 50

// analogic
// I2C

#define PIN_TEMSENSOR A1
#define PIN_POTA A2
#define PIN_POTB A3

// Se asume dado la arquitectura de la placa.
#define PIN_I2C_SDA A4
#define PIN_I2C_SCL A5


// Filas y Columnas de la pantalla LCD
# define LCD_ROWS 2
# define LCD_COLUMNS 16


# define MODE1 1
# define MODE2 2
# define MODE3 3


const int btnRIGHT=0;
const int btnUP=1;
const int btnDOWN=2;
const int btnLEFT=3;
const int btnSELECT=4;
const int btnNONE=5;
