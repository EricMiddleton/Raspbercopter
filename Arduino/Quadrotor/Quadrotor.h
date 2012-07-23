#pragma once
#ifndef QUADROTOR_H
#define QUADROTOR_H

#include <Arduino.h>
#include <Wire.h>

#include "utility\Datatypes.h"
#include "utility\Model.h"
#include "utility\Locator.h"
#include "utility\Communicator.h"
#include "utility\Battery.h"
#include "utility\Altimeter.h"

class Quadrotor
{
public:
	//Constructor
	void Init(int mass, int length, int thrust, int mAH);

	//Main Quadrocopter loop
	void Update();

	//Physics Model functions
	Vector GetMOI();
	void SetMOI(Vector MOI);

	//IMU functions
	Angle GetOrientation();

	//Locator functions
	float GetAltitude();
	Vector GetPosition();
	Vector GetVelocity();
	float GetGPSAccuracy();

	//Altimeter functions
	//TODO

	//Communicator functions
	void SendMessage(char *message);
	void SendReceiveMessage(char *message, char *response);

	//Battery functions
	int GetBatteryLevel();
	void SetBatteryAlarmLevel(int level);
	boolean GetBatteryAlarm();

private:
	static void InitTimer();
	Model model;
	Locator locator;
	//Communicator communicator;
	//Battery battery;

	int lastTime;
};

//We need a pointer to our quadrotor
//For or Interrupt Service Routine
//To access
extern Quadrotor		*_quad;
extern Model			*_model;
extern unsigned long	_timerStartVal;
extern byte				_count;
#endif