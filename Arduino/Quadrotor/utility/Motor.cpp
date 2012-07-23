#pragma once
#include "Motor.h"

void Motor::Init(byte addr, unsigned int maxThrust) {
	this->maxThrust = maxThrust;

	thrust = 0;

	multiplier = (float)1000 / maxThrust;

	controller.attach(addr);
}

void Motor::SetThrust(unsigned int thrust) {
	
	this->thrust = min(thrust, maxThrust);
	controller.writeMicroseconds((unsigned int)(this->thrust*multiplier + 1000));
}

float Motor::GetThrust() {
	return thrust;
}

unsigned int Motor::GetMaxThrust() {
	return maxThrust;
}

