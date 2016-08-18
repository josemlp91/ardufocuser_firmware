#include <AccelStepper.h>

#define PINDIR 3
#define PINSTEP 2

AccelStepper motor(1, PINSTEP, PINDIR);

void setup() {
  Serial.begin(9600);
  motor.setMaxSpeed(500);
  motor.setAcceleration(100);


}

void loop() {
   motor.run();
   motor.moveTo(-3000);

}
