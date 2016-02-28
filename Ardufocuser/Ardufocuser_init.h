/// Inicializamos el sistema.

// Iniciamos nunchuck
//WiiChuck chuck = WiiChuck();

// Inicializa controlador POLOLU
//AccelStepper motor(1, PINSTEP, PINDIR);

// Inicializa LCD I2C en canal 27.
LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);

// Variables para controlar los limites findes de carrera hardware/software.
bool limitRunActiveA=false; bool limitRunActiveB=false;
bool limitRunSoftwareActiveA=false; bool limitRunSoftwareActiveB=false;
int  limitRunSoftwareA=-300; int  limitRunSoftwareB=300;

// Variable para controlar velocidad.
int speed = 0;

// Variable para controlar pasos por cada pulso.
int stepPerPulse = 0;

// Variable que controla la posición del motor en cada


// Variables para controlar la temperatura.
int temperature=0;

// Tiempos del último refesco.
unsigned long lastTimeUpdate=0;

// Intervalo de refresco 1 segundo.
int refresh_rate=1000;

int mode = 1;

// Variables auxiliares.
int lastspeed = 0;
int laststepPerPulse = 0;
bool hadToReadspeed=true;
bool hadToReadstepPerPulse=true;