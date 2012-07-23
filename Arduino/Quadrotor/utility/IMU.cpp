#pragma once
#include "IMU.h"

#define    AXIS_THRESHOLD      0.1

Angle IMU::GetOrientation() {
  return orientation;
}

IMU::IMU() {
}

void IMU::Init()
{
  //Calculate measurement accuracies
  probGyro = Angle(0.98, 1, 0.98);
  probStatic = Angle(0.02, 0, 0.02);
  
  //Initialize Accelerometer
  accelerometer = Accelerometer();
  accelerometer.Init();
  
  //Initialize Gyroscope
  gyroscope = Gyroscope();
  gyroscope.Init();
  
  //Initialize Compass
  //magnetometer((byte)0x1E);
  
  //Gather and Calculate inital values
  accelerometer.Get(&acceleration);
  //magnetometer.Get(&magneticField);
  gyroscope.Get(&angularVelocity);

  //We'll assume a zero starting angle
  //If not, it will correct itself in a few seconds
  //thanks to the accelerometer
  orientation = Angle(0, 0, 0);
}

void IMU::Update()
{
  //Local Variables
  Angle newAngVel, staticAng;

  //Gather sensor measurements
  accelerometer.Update();
  gyroscope.Update();
  accelerometer.Get(&acceleration);
  //magnetometer.Get(&magneticField);
  gyroscope.Get(&newAngVel);

  //Calculate Angle from gravity vector
  //TODO: incorporate magnetometer to calculate yaw
  staticAng = acceleration.ToAngle(GRAVITY);
  
  //Estimate position from static angle and the gyroscope integration
  //Magic value '0.01f' is dt
  orientation = staticAng;//((orientation + (angularVelocity + newAngVel)*0.01f)*probGyro + staticAng*probStatic);
  
  //Keep all angles between -PI and PI
  orientation.wrap();
  
  //save angular velocity
  angularVelocity = newAngVel;
}
