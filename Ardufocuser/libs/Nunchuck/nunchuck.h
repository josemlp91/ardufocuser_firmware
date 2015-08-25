#define POWER_VIA_PORT_C2_C3 1

#define USE_NEW_WAY_INIT 1 
#define WII_IDENT_LEN ((byte)6)
#define WII_TELEGRAM_LEN ((byte)6)
#define WII_NUNCHUCK_TWI_ADR ((byte)0x52)

#include "WProgram.h"
#include <string.h> 
#include <utilitytwi.h>
#undef int
#include <stdio.h>

static uint8_t outbuf[WII_TELEGRAM_LEN];	 // array to store arduino output
int cnt = 0;

// -------- output -------
byte NunC_JX = 0;
byte NunC_JY = 0;
byte NunC_AX = 0;
byte NunC_AY = 0;
byte NunC_AZ = 0;
boolean NunC_BC = 0;
boolean NunC_BZ = 0;

// ===========================================================
// ================  Private Functions  ======================
// ==========================================================

byte readControllerIdent(byte* pIdent)
{
	byte rc = 1;

	// read identification
	Wire.beginTransmission (WII_NUNCHUCK_TWI_ADR);	// transmit to device 0x52
	Wire.send (0xFA);	 // sends memory address of ident in controller
	if(Wire.endTransmission () == 0)	// stop transmitting
	{
		byte i;
		Wire.requestFrom (WII_NUNCHUCK_TWI_ADR, WII_TELEGRAM_LEN);	// request data from nunchuck
		for (i = 0; (i < WII_TELEGRAM_LEN) && Wire.available (); i++) 		{ 			pIdent[i] = Wire.receive();	// receive byte as an integer 		} 		if(i == WII_TELEGRAM_LEN) 		{ 			rc = 0; 		} 	} 	return rc; } void clearTwiInputBuffer(void) { 	// clear the receive buffer from any partial data 	while( Wire.available ()) 		Wire.receive (); } // Decode data format that original Nunchuck uses with old init sequence. This never worked with // other controllers (e.g. wireless Nunchuck from other vendors) char nunchuk_decode_byte (char x) { #ifndef USE_NEW_WAY_INIT 	x = (x ^ 0x17) + 0x17; #endif 	return x; } void ConvertOutputs(){      /*   Joy - X 0 137 255      [0]         Y 0 142 255      [1]   Acc - X 290  560  715  [2]  (*2*2)         Y 320  535  750  [3]  (*2*2)   */   // will use Joy X, Y for Coxa and femur, Acc Y for Tibia   /*   PosX = outbuf[1]/4;   PosZ = (outbuf[0]-128)/2;   PosY = (outbuf[2] - 72)/2;   */      NunC_JX = outbuf[0];   NunC_JY = outbuf[1];   NunC_AX = map(outbuf[2]*2*2, 256, 768, 0, 255);   NunC_AY = map(outbuf[3]*2*2, 256, 768, 0, 255);   NunC_AZ = map(outbuf[4]*2*2, 256, 768, 0, 255);   if ((outbuf[5] >> 0) & 1)  NunC_BZ = 0;
  else                       NunC_BZ = 1;
  if ((outbuf[5] >> 1) & 1)  NunC_BC = 0;
  else                       NunC_BC = 1;

  // buffer 5 also contains the least significant bit of accelerometer data
  if ((outbuf[5] >> 2) & 1)  NunC_AX += 2;
  if ((outbuf[5] >> 3) & 1)  NunC_AX += 1;
  if ((outbuf[5] >> 4) & 1)  NunC_AY += 2;
  if ((outbuf[5] >> 5) & 1)  NunC_AY += 1;
  if ((outbuf[5] >> 6) & 1)  NunC_AZ += 2;
  if ((outbuf[5] >> 7) & 1)  NunC_AZ += 1;

}

// ===========================================================
// ================  Public Functions  ======================
// ==========================================================

// params:
// timeout: abort when timeout (in ms) expires, 0 for unlimited timeout
// return: 0 == ok, 1 == timeout
void NunC_Init (unsigned short timeout)
{

  // ----------------------- setup power pins -----------------------

    #ifdef POWER_VIA_PORT_C2_C3 // power supply of the Nunchuck via port C2 and C3
	PORTC &=~ _BV(PORTC2);
	PORTC |= _BV(PORTC3);
	DDRC |= _BV(PORTC2) | _BV(PORTC3); // make outputs
	delay(100); // wait for things to stabilize
    #endif

  // ----------------------- timer speed? -----------------------
	Wire.begin(); // initialize i2c
	// we need to switch the TWI speed, because the nunchuck uses Fast-TWI
	// normally set in hardwarelibrariesWireutilitytwi.c twi_init()
	// this is the way of doing it without modifying the original files
    #define TWI_FREQ_NUNCHUCK 400000L
	TWBR = ((CPU_FREQ / TWI_FREQ_NUNCHUCK) - 16) / 2;

	byte rc = 1;

  // ----------------------- initialize nunchuck -----------------------
    #ifndef USE_NEW_WAY_INIT
	// look at <http://wiibrew.org/wiki/Wiimote#The_Old_Way> at "The Old Way"
	Wire.beginTransmission (WII_NUNCHUCK_TWI_ADR);	// transmit to device 0x52
	Wire.send (0x40);	 // sends memory address
	Wire.send (0x00);	 // sends sent a zero.
	Wire.endTransmission ();	// stop transmitting
    #else
	// disable encryption
	// look at <http://wiibrew.org/wiki/Wiimote#The_New_Way> at "The New Way"

	unsigned long time = millis();
	do
	{
		Wire.beginTransmission (WII_NUNCHUCK_TWI_ADR);	// transmit to device 0x52
		Wire.send (0xF0);	 // sends memory address
		Wire.send (0x55);	 // sends data.
		if(Wire.endTransmission() == 0) // stop transmitting
		{
			Wire.beginTransmission (WII_NUNCHUCK_TWI_ADR);	// transmit to device 0x52
			Wire.send (0xFB);	 // sends memory address
			Wire.send (0x00);	 // sends sent a zero.
			if(Wire.endTransmission () == 0)	// stop transmitting
			{
				rc = 0;
			}
		}
	}
	while (rc != 0 && (!timeout || ((millis() - time) < timeout)));
      #endif

  // ----------------------- finish setup -----------------------
	// display the identification bytes, must be "00 00 A4 20 00 00" for the Nunchuck
	byte i;
	if(readControllerIdent(outbuf) == 0)
	{
		Serial.print("Ident=");
		for (i = 0; i < WII_TELEGRAM_LEN; i++)
		{
			Serial.print(outbuf[i], HEX);
			Serial.print(' ');
		}
		Serial.println();
	}

	Serial.println("Finished setup");

}

boolean NunC_RecieveCommand(){
	Wire.requestFrom (WII_NUNCHUCK_TWI_ADR, WII_TELEGRAM_LEN);	// request data from nunchuck

	for (cnt = 0; (cnt < WII_TELEGRAM_LEN) && Wire.available (); cnt++) 		outbuf[cnt] = nunchuk_decode_byte (Wire.receive ());	// receive byte as an integer 	clearTwiInputBuffer(); 	// If we recieved the 6 bytes, then go print them 	if (cnt >= WII_TELEGRAM_LEN){
          ConvertOutputs();
          return true;
        }
        else                          
          return false;
}

void NunC_SendNextByteRequest()
{
	// I don't know why, but it only works correct when doing this exactly 3 times
	// otherwise only each 3rd call reads data from the controller (cnt will be 0 the other times)
	for(byte i = 0; i < 3; i++)
	{
		Wire.beginTransmission (WII_NUNCHUCK_TWI_ADR);	// transmit to device 0x52
		Wire.send (0x00);	 // sends one byte
		Wire.endTransmission ();	// stop transmitting
	}
}

void print ()
{

     Serial.print (NunC_JX, DEC);
     Serial.print ("t");

     Serial.print (NunC_JY, DEC);
     Serial.print ("t");

     Serial.print (NunC_AX, DEC);
     Serial.print ("t");

     Serial.print (NunC_AY, DEC);
     Serial.print ("t");

     Serial.print (NunC_AZ, DEC);
     Serial.print ("t");

     Serial.print (NunC_BZ, DEC);
     Serial.print ("t");

     Serial.print (NunC_BC, DEC);
     Serial.print ("t");

     Serial.print ("rn");
}
