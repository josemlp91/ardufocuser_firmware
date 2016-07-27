///////////////////////////////////////////////////////////////////////////////
///////////////////////////// SESIONES PERESISTENTES //////////////////////////

// Estructura para almacenar los valores de la sesion actual.
struct RecControlConfig {
  
  int speed;
  int aceleration;
  int stepPerPulse;
  int position;
  int microStep;
  int temperature;
  int mode;
  int count;
  int limitRunSoftwareA;
  int limitRunSoftwareB;
  bool lcd_lux_status;

};

///////////////////////////////////////////////////////////////////////////////////
// Instancias de los modulos de los distintos perifericos.
///////////////////////////////////////////////////////////////////////////////////

// Iniciamos nunchuck
WiiChuck chuck = WiiChuck();

// Inicializa controlador POLOLU
AccelStepper motor(1, PINSTEP, PINDIR);

// Inicializa LCD I2C en canal 27.
LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);

// Instancia de SerialCommand, tokenizador y parseador de puerto serie.
SerialCommand serial_cmd; 

// Estructura para almacenar los valores de la sesion actual.
RecControlConfig rec_config;

///////////////////////////////////////////////////////////////////////////////////
// Variables para controlar los limites findes de carrera hardware/software.
///////////////////////////////////////////////////////////////////////////////////
bool limitRunActiveA=false; bool limitRunActiveB=false;
bool limitRunSoftwareActiveA=false; bool limitRunSoftwareActiveB=false;
int  limitRunSoftwareA=-300; int  limitRunSoftwareB=300;


///////////////////////////////////////////////////////////////////////////////////
// Variables generales.
// 
// TODO: Cargar desde EEPROM
///////////////////////////////////////////////////////////////////////////////////
// Variable para controlar velocidad.
int speed = 0;

// Variable para controlar pasos por cada pulso.
int stepPerPulse = 0;

// Variable que controla la posición del motor en cada
int position=0;

// Temperatura actual.
int temperature=0;

// Temperatura ultimo ajuste del foco.
int temp_perform=0;

// Variables para controlar la temperatura.
int aceleration=0;

// Configuración de micropasos.
int microStep=1;

// Estado del led de la pantalla.
bool lcd_lux_status=1;

// Boton pulsado en el momento.
int button=btnNONE;

// Modo de funcionamiento.
int mode = ONLY_MANUAL;


///////////////////////////////////////////////////////////////////////////////////
// Tiempos del último refesco.
/////////////////////////////////////////////////////////////////////////////////////
unsigned long lastTimeReadManualController=0;
int manual_controller_refresh_rate=250;

unsigned long lastTimeUpdateLCD=0;
int lcd_refresh_rate=100;

unsigned long lastTimeCheckChangeTemp=0;
int temp_refresh_rate=100;

unsigned long lastTimeComunicatePosition=0;
int current_position_refresh_rate=250;

unsigned long lastTimeComunicateTemperature=0;
int current_temperature_refresh_rate=1000;

unsigned long lastTimeComunicateSLimit=0;
int current_s_limit_refresh_rate=1000;

unsigned long lastTimeReadNunchuckLimit=0;
int nunchuck_refresh_rate=500;


unsigned long lastTimeComunicateRun=0;
int current_run_refresh_rate=2000;


unsigned long lastTimeSaveSession=0;
int save_sesion_refresh_rate=1000 * 10 * 1; // Cada 1/2 minuto.


int lastPulse=btnSELECT;
unsigned long lastTimeUpdate=0;

// Intervalo de refresco 1 segundo.
int refresh_rate=1000;

// Variables auxiliares.
int lastspeed = 0;
int laststepPerPulse = 0;

bool hadToReadspeed=true;
bool hadToReadstepPerPulse=true;
//////////////////////////////////////////////////////////////////////////////////////////7

