/*Written by Jorge Sánchez Encinas*/
#include <Arduino.h>
#include <ESP32Servo.h>

Servo servo_0;
Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_6;
Servo servo_4;
Servo servo_5;
unsigned long ledPreviousTime = 0; 

const int shoulderOffset = -4;
const int elbowOffset = -8;
const byte twistRistOffset = 16;
const byte pitchWristOffset = 12;

//Variables for millis function
unsigned long currentTime;
byte ledStepFlag = 0;

//byte buttonValue;
//float buttonValueSmoothed;
//float buttonPrevious;
float rotationAngleSmoothed;
float rotationAnglePrevious;

float x;
float y;
byte currentCuadrant; //(I, II, III, IV)

//Begining of angle calculations according to x, y, z coordinates
float rotationAngle = 90;
//End of angle calculations according to x, y, z coordinates

void setup() {
  Serial.begin(115200);
  pinMode(15, OUTPUT);
  pinMode(22, INPUT_PULLUP);

  servo_1.attach(2);
  servo_2.attach(4);
  servo_3.attach(5);
  servo_4.attach(18);
  servo_5.attach(19);
  servo_6.attach(21);

//Servo's initial position
  servo_1.write(90);
  servo_2.write(90 + shoulderOffset);
  servo_3.write(90 + elbowOffset);
  delay(10);
  servo_4.write(90 + twistRistOffset);
  servo_5.write(90 + pitchWristOffset);
  servo_6.write(110);

}

void ledBlink(){
  if(rotationAngle <= servo_1.read() - 3.5 || rotationAngle >= servo_1.read() + 3.5){
    ledStepFlag = 0;
    if(currentTime - ledPreviousTime >= 80UL){
      digitalWrite(15, HIGH);
    }
    if(currentTime - ledPreviousTime >= 160UL){
      digitalWrite(15, LOW);
      ledPreviousTime = currentTime;
    }
  }
  else{
    if(ledStepFlag == 0 && currentTime - ledPreviousTime >= 80UL){
      digitalWrite(15, HIGH);
      ledStepFlag = 1;
      ledPreviousTime = currentTime;
    }
    else if(ledStepFlag == 1 && currentTime - ledPreviousTime >= 80UL){
      digitalWrite(15, LOW);
      ledStepFlag = 2;
      ledPreviousTime = currentTime;
    }
    else if(ledStepFlag == 2 && currentTime - ledPreviousTime >= 80UL){   
      digitalWrite(15, HIGH);
      ledStepFlag = 3;
      ledPreviousTime = currentTime;
    }
    else if(ledStepFlag == 3 && currentTime - ledPreviousTime >= 80UL){   
      digitalWrite(15, LOW);
      ledStepFlag = 4;
      ledPreviousTime = currentTime;
    }
    else if(ledStepFlag == 4 && currentTime - ledPreviousTime >= 920UL){   
      digitalWrite(15, LOW);
      ledStepFlag = 0;
      ledPreviousTime = currentTime;
    }
  }
}

void moveServoMotorWithCoordinates(){
//Begining of smoothing the angle
  rotationAngleSmoothed = (rotationAngle * 0.003) + (rotationAnglePrevious * 0.997);
  rotationAnglePrevious = rotationAngleSmoothed;
//End of smoothing the angle
  servo_1.write(rotationAngleSmoothed);
}

void angleCalculations(){
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    x = input.substring(1,5).toInt();
    y = input.substring(6,10).toInt();
    Serial.print(x);
    Serial.print(" , ");
    Serial.println(y);
//Begining of localization of CUADRANT on the plane (I, II, III, IV)
    rotationAngle = 180 * (atan(abs(y/x))) / PI;
    if(x>=0){
      if(y>0){    //If angle is located on the ++ CUADRANT of the plane CUADRANT I
        currentCuadrant = 1;
      }
      else if(y<=0){   //If angle is located on the +- CUADRANT of the plane CUADRANT IV 
        rotationAngle = 180 - rotationAngle;
        currentCuadrant = 4;
      }
    }
    else if(x<0){
      if(y>=0){   //If angle is located on the -+ CUADRANT of the plane CUADRANT II
        rotationAngle = 180 - rotationAngle;
        currentCuadrant = 2;
      }
      else if(y<0){  //If angle is located on the -- CUADRANT of the plane CUADRANT III
        currentCuadrant = 3;
      }
    }
//End of localization of CUADRANT on the plane (I, II, III, IV)
    Serial.println(rotationAngle);
    Serial.println("Cuadrant:" + String(currentCuadrant));
  }
}

void loop() {
  currentTime = millis();
  ledBlink();
  angleCalculations();
  moveServoMotorWithCoordinates();
  // Serial.print(buttonValueSmoothed);
  Serial.print(rotationAngle);
  Serial.print(" , ");
  Serial.println(servo_1.read());
}
