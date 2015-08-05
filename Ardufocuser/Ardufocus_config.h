
#define MAXSTEPPXPULSA   100

#define MAXVEL   500
#define MINVEL   50
#define MAXACC   0
#define MINACC   500
#define DEF_ACC  300 

#define PINDIR 44
#define PINSTEP 42

#define PIN_BUTTON A0
#define PIN_POTA A1
#define PIN_POTB A3

//#define PIN_LIMITRUNA 2  
//#define PIN_LIMITRUNB 1 //not defined

#define PIN_TEMSENSOR A2

#define PINLED_WARNING 50

#define PINLCD_RS 8
#define PINLCD_ENABLE 9
#define PINLCD_D4 4
#define PINLCD_D5 5
#define PINLCD_D6 6
#define PINLCD_D7 7

#define BRIGHTNESS 20
#define PINBRIGHTNESS 10

#define MAX_CHANGE_TEMP 2 

#define LCD_RATE_UPDATE 100

//enum WorkMode {REMOTE, MANUAL, MIX};

const int btnRIGHT=0;
const int btnUP=1;
const int btnDOWN=2; 
const int btnLEFT=3;
const int btnSELECT=4; 
const int btnNONE=5;

/*

enum ArdufocusCmd {AINIT, AMODE, AG, APOSITION, ATEMP, ALTEMP, AMICRO, AFINE, ASPEED,
                       AACC, AR, AHLIMIT,ASLIMIT, ASILIMIT, ASOLIMIT, AVERS, AMOV,
                       ARUNA, ARUNB, ASTOP};   //comando de prueba.   

*/
//enum btnLCD {btnRIGHT, btnUP, btnDOWN, btnLEFT, btnSELECT, btnNONE};
