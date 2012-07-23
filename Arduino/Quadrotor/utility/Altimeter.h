#ifndef ALTIMETER_H
#define ALTIMETER_H

#include "Sensors.h"
#include <Wire.h>
#include <Arduino.h>

class Altimeter
{
public:
	void					Init();
	void					UpdateRanger();
	void					UpdateBarometer();
	float					GetRangerAltitude(Angle orientation);
	float					GetBarometerAltitude();
private:
	//Barometer				barometer;
	Ranger					ranger;
	unsigned long			seaPressure;
	float					altitude;

};

extern Ranger				*_ranger;

#endif