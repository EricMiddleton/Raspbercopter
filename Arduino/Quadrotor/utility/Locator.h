#pragma once
#ifndef LOCATOR_H
#define LOCATOR_H

#include <Arduino.h>
#include <string.h>
#include <avr/interrupt.h>
#include "Datatypes.h"

class Locator {
  public:
    void             Init();
    void             Update();
    Vector           GetLocation();
    Vector           GetVelocity();
    float            GetAltitude();
    float            GetHeading();
	Vector           GetPosition();
	float            GetGPSAccuracy();
	void             Parse(char *message);

  private:
    Vector           location;
	Vector           longLat;
    Vector           velocity;
    float            altitude;
    float            heading;
	float            gpsAccuracy;
    unsigned long    lastTime;
	boolean          fix;
};

extern char          message[256];
extern byte          ptr;
void serialEvent1();

#endif