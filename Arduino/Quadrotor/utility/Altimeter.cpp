#ifndef ALTIMETER_CPP
#define ALTIMETER_CPP

#include "Altimeter.h"
#include <Arduino.h>
#include <Wire.h>

void Altimeter::Init() {
	ranger.Init();

	//barometer();
	seaPressure = 101325;
}

void Altimeter::UpdateRanger() {
	ranger.Update();
}

void Altimeter::UpdateBarometer() {
	//altitude = barometer.readAltitude(seaPressure);
}

float Altimeter::GetRangerAltitude(Angle orientation) {
	float pitch = orientation.GetPitch(), roll = orientation.GetRoll(), raw, cosine = cos(sqrt(pitch*pitch + roll*roll));

	if(!cosine)
		return -1;
	else {
		ranger.Get(&raw);
		return raw / cosine;
	}
}

float Altimeter::GetBarometerAltitude() {
	return 0;//altitude;
}
#endif