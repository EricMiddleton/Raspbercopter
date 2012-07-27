#pragma once
#ifndef MODEL_H
#define MODEL_H

#define PINBASE		9

#include "Datatypes.h"
#include <Arduino.h>
#include <Servo.h>
#include "IMU.h"
#include "Altimeter.h"

class PID
{
public:
	void Init(float p, float i, float d);
	void SetPID(float p, float i, float d);
	Vector GetPID();
	float Update(float wanted, float current);
private:
	float p, i, d;
	float error, dError, sError;
};

class Model
{
public:
	void Init(unsigned int mass, unsigned int thrust);
	Vector GetPID();
	void SetPID(Vector v);
	Angle GetAngle();
	Angle GetOrientation();
	void SetAngle(Angle a);
	void Update();
private:
	Servo motor1, motor2, motor3, motor4;
	IMU imu;
	Altimeter altimeter;
	PID pid1, pid2, pid3, pid4;
	unsigned int mass, thrust;
	float multiplier;
	Angle angle;
};

#endif