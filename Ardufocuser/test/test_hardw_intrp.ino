int led = 13;

void setup()
   {   Serial.begin(9600);
       pinMode(led, OUTPUT);
       // Interrupcion harware pin 2
       attachInterrupt( 0, ServicioBoton, RISING);
   }
void loop()
   {
     digitalWrite(led, LOW);

   }

void ServicioBoton()
   {
   digitalWrite(led, HIGH);
   Serial.println("boton");
   }
