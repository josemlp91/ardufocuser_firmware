///////////////////////////////////////////////////////////////////////////////////
// Instancias de los modulos de los distintos perifericos.
///////////////////////////////////////////////////////////////////////////////////

// Iniciamos nunchuck
WiiChuck chuck = WiiChuck();

// Inicializa controlador POLOLU
AccelStepper motor(1, PINSTEP, PINDIR);

// Inicializa LCD I2C en canal 27.
LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);


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


int lastPulse=btnSELECT;
unsigned long lastTimeUpdate=0;

// Intervalo de refresco 1 segundo.
int refresh_rate=1000;

// Variables auxiliares.
int lastspeed = 0;
int laststepPerPulse = 0;


bool hadToReadspeed=true;
bool hadToReadstepPerPulse=true;


