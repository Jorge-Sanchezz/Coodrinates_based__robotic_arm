/*Written by Jorge Sánchez Encinas*/
#include <Arduino.h>
#include <ESP32Servo.h>
#include "Led.h"

Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;
Servo servo_5;
Servo servo_6;

Led led(15);

void setup() {
  led.begin();
  Serial.begin(115200);
}


void loop() {
  //angleCalculations();
  //moveServoMotorWithCoordinates();
  //ledBlink();
  led.stand_by_mode();

}