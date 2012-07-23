#pragma once
#ifndef MOTOR_H
#define MOTOR_H
#include <WProgram.h>
#include <Arduino.h>
#include <Servo.h>

//Motor 0 connects to pin 9, Motor 1 to pin 10, etc.
#define PINBASE		9

class Motor
{
public:
	void Init(byte addr, unsigned int maxThrust);
	void SetThrust(unsigned int thrust);
	float GetThrust();
	unsigned int GetMaxThrust();
private:
	Servo controller;
	float multiplier;
	unsigned int maxThrust, thrust;
};
#endif