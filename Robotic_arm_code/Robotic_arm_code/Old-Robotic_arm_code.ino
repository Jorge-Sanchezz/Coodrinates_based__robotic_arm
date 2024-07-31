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
float z;
byte currentCuadrant; //(I, II, III, IV)

//Begining of angle calculations according to x, y, z coordinates
float rotationAngle = 90;
float shoulderAngle = 90;
float elbowAngle = 90;
float wristPitchAngle = 90;


float gripperAngle = 152;
int gripperLength = 85;


float hST1; //Hypotenuse of Subtriangle 1
float hST2; //Hypotenuse of Subtriangle 2
float K; //Angle located in  Sub Triangle 2 and Triangle 1 (angle mirrored)
float ak; //Line which is divisor in between Triangle 1 and Triangle 2
float BT1;
float AT1;
float AT2;
float BT2;
float CT2;
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
  servo_2.write(90 + shoulderOffset);
  delay(300);
  servo_1.write(90);
  servo_3.write(90 + elbowOffset);
  delay(200);
  servo_4.write(90 + twistRistOffset);
  servo_5.write(90 + pitchWristOffset);
  servo_6.write(gripperAngle); //70-172
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
  servo_2.write(shoulderAngle + shoulderOffset);
  servo_3.write(elbowAngle + elbowOffset);

  servo_5.write(wristPitchAngle + pitchWristOffset);
  servo_6.write(gripperAngle);
}
void angleCalculations(){
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    x = input.substring(1,5).toInt();
    y = input.substring(6,10).toInt();
    z = input.substring(11,15).toInt();
    Serial.print(x);
    Serial.print(" , ");
    Serial.print(y);
    Serial.print(" , ");
    Serial.println(z);
//Beginning of localization of CUADRANT on the plane (I, II, III, IV)
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


//Beginning of calculations to get the current length of the gripper (as it oscillates depending on it's state)
  gripperLength = -0.357 * gripperAngle + 150;
  //Gripper limits
  if(gripperAngle >= 172){gripperAngle = 172;}
  if(gripperAngle <= 70){gripperAngle = 70;}
//End of calculations to get the current length of the gripper (as it oscillates depending on it's state)


//Beginning of calculations to get the angles required for the servo 2,3,4
  hST1 = sqrt(sq(x) + sq(y));
  hST2 = sqrt(sq(hST1) + sq(z));
  K = sin(abs(z/hST1));
  ak = sqrt(sq(gripperLength) +  sq(hST2) - 2*(gripperLength)*(hST2) * cos(K));
  
  BT1 = asin((hST2*sin(K)) / ak);
  AT1 = PI - (BT1 + K);
  AT2 = acos((-sq(ak) + sq(120) + sq(120)) / (2 * 120 * 120));
  BT2 = asin((sin(AT2) * 120) / ak);
  CT2 = PI - (AT2 + BT2);

  shoulderAngle = 180 - (((K + AT1 + CT2) * (180 / PI)) / 2);
  elbowAngle = 180 - ((AT2) * (180 / PI)) - 90;
  wristPitchAngle = (BT2 + BT1) * (180 / PI) +90;
//End of calculations to get the angles required for the servo 2,3,4

    Serial.print("Rotation angle : ");
    Serial.println(rotationAngle);

    Serial.print("Shoulder angle : ");
    Serial.println(shoulderAngle);

    Serial.print("Elbow angle : ");
    Serial.println(elbowAngle);

    Serial.print("Wrist Pitch angle : ");
    Serial.println(wristPitchAngle);

    Serial.println("Cuadrant:" + String(currentCuadrant));

    Serial.print("hST1: ");
    Serial.println(hST1);

    Serial.print("hST2: ");
    Serial.println(hST2);

    Serial.print("K: ");
    Serial.println(K);

    Serial.print("ak: ");
    Serial.println(ak);

    Serial.print("BT1: ");
    Serial.println(BT1);

    Serial.print("AT1: ");
    Serial.println(AT1);

    Serial.print("AT2: ");
    Serial.println(AT2);

    Serial.print("BT2: ");
    Serial.println(BT2);

    Serial.print("CT2: ");
    Serial.println(CT2);

    Serial.print("gripperLength: ");
    Serial.println(gripperAngle);
  }
}

void loop() {
  currentTime = millis();
  ledBlink();
  angleCalculations();
  moveServoMotorWithCoordinates();
  // Serial.print(buttonValueSmoothed);

  // Serial.print(rotationAngle);
  // Serial.print(" , ");
  // Serial.println(servo_1.read());

  int potenciometerValue = map(analogRead(13), 4095, 1930, 0, 180);
  //Serial.println(potenciometerValue);
}
