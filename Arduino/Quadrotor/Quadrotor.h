#pragma once
#ifndef QUADROTOR_H
#define QUADROTOR_H

#include <Arduino.h>
#include <Wire.h>

#include "utility/Datatypes.h"
#include "utility/Model.h"
#include "utility/Locator.h"
#include "utility/Communicator.h"
#include "utility/Battery.h"
#include "utility/Altimeter.h"

class Quadrotor
{
public:
	//Constructor
	Quadrotor();
	void Init(unsigned int mass, byte length, unsigned int thrust, unsigned int mAH);

	//Main Quadrocopter loop
	void Update();

	//IMU functions
	Angle GetOrientation();

	//Locator functions
	/*float GetAltitude();
	Vector GetPosition();
	Vector GetVelocity();
	float GetGPSAccuracy();*/

	//Altimeter functions
	//TODO

	//Communicator functions
	//void SendMessage(char *message);
	//void SendReceiveMessage(char *message, char *response);

	//Battery functions
	//int GetBatteryLevel();
	//void SetBatteryAlarmLevel(int level);
	//boolean GetBatteryAlarm();

private:
	static void InitTimer();
	Model model;
	//Locator locator;
	//Communicator communicator;
	//Battery battery;
};

//We need a pointer to our quadrotor
//For or Interrupt Service Routine
extern Quadrotor		*_quad;
extern Model			*_model;
extern unsigned int		_timerStartVal;
extern byte				_count;
#endif