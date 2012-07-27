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

  //We'll assume a zero starting angle
  //If not, it will correct itself in a few seconds
  //thanks to the accelerometer
  orientation = Angle(0, 0, 0);
  lastTime = micros();

  Update();
  Serial.println(F("Update"));
}

void IMU::Update()
{
  //Local Variables
  Angle newAngVel, staticAng;
  unsigned int newTime;
  float dt;

  //Gather sensor measurements
  newTime = micros();
  accelerometer.Update();
  gyroscope.Update();
  accelerometer.Get(&acceleration);
  gyroscope.Get(&newAngVel);
  dt = (newTime - lastTime)*0.000001f;

  //Calculate Angle from gravity vector
  //TODO: incorporate magnetometer to calculate yaw
  staticAng = acceleration.ToAngle(GRAVITY);
  
  //Estimate position from static angle and the gyroscope integration
  //Magic value '0.01f' is dt
  orientation = ((orientation + (angularVelocity + newAngVel)*dt)*probGyro + staticAng*probStatic);
  
  //Keep all angles between -PI and PI
  orientation.wrap();
  
  //save angular velocity
  angularVelocity = newAngVel;
  lastTime = newTime;
}
