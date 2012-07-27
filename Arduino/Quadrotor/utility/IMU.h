#pragma once
#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include "Datatypes.h"
#include "Sensors.h"

class IMU
{
public:
					IMU();
  void				Init();
  Vector			GetAcceleration();
  Angle				GetGyroscope();
  Vector			GetMagnetometer();
  Angle				GetOrientation();
  void				Update();
private:
  Accelerometer		accelerometer;
  Gyroscope			gyroscope;
  //Magnetometer		magnetometer;
  Vector			acceleration;//, magneticField;
  Angle				angularVelocity, orientation, probGyro, probStatic;
  unsigned int		lastTime;
};
#endif