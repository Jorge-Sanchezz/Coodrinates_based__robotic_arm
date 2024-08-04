/*
  InverseKinematics.h - Library for calculating each servo's angle
  Created by Jorge E. Sánchez
*/
#ifndef InverseKinematics_h
#define InverseKinematics_h

#include "Arduino.h"

class InverseKinematics {
  public:
    InverseKinematics(double armLength, double forearmLength);
    void calculate_IK(float x, float y, float z, int gripperState);
    double servo_1_angle();
    double servo_2_angle();    
    double servo_3_angle();
    double servo_4_angle();
    double servo_5_angle();
    double servo_6_angle();
    
  private:
    double _armLength, _forearmLength, _gripperState;
    float _x, _y, _z;
};

#endif
