#pragma once
#include "Locator.h"

Locator *_locator;
char message[256];
byte ptr;

Vector Locator::GetLocation() {
  return this->location;
}
Vector Locator::GetVelocity() {
  return this->velocity;
}
float Locator::GetAltitude() {
  return this->altitude;
}
float Locator::GetHeading() {
  return this->heading;
}
Vector Locator::GetPosition() {
	//TODO: Actually write this part!
	return Vector(0, 0, 0);
}
float Locator::GetGPSAccuracy() {
	return gpsAccuracy;
}

void Locator::Init() {
	fix = 0;
	ptr = 0;

	_locator = this;

	Serial1.begin(115200);
	UCSR1B |= (1 << RXCIE1);

  lastTime = millis();
  velocity = Vector(0, 0, 0);
}

void Locator::Update() {
  float curTime = millis(), dt = (curTime - lastTime)*0.001;
  
  //TODO: things requiring GPS (x/y location, x/y velocity, heading)
  lastTime = curTime;
}

void Locator::Parse(char *message) {
	//If this is message contains the position
	if(!memcmp(message, "$GPRMC", 6)) {
		boolean valid = 0, north = 0, east = 0;
		char lon[16], lat[16], speed[16], course[16];
		for(byte n = 0, i = 0, ptr = 0; n < 255, i < 8; n++) {

			char ch = message[n];

			if(ch == ',') {
				if(i == 2)
					lat[ptr] = '\0';
				else if(i == 4)
					lon[ptr] = '\0';
				else if(i == 6)
					speed[ptr] = '\0';
				else if(i == 7)
					speed[ptr] = '\0';
				i++;
				ptr = 0;
			}
			else if(i == 1) {
				valid = (ch == 'A');
			}
			else if(i == 2) {
				lat[ptr++] = ch;
			}
			else if(i == 3) {
				north = (ch == 'N');
			}
			else if(i == 4) {
				lon[ptr++] = ch;
			}
			else if(i == 5) {
				east = (ch == 'E');
			}
			else if(i == 6) {
				speed[ptr++] = ch;
			}
			else if(i == 7) {
				course[ptr++] = ch;
			}
		}
		Serial.println();
		Serial.print(lon);
		if(north)
			Serial.print("N, ");
		else
			Serial.print("S, ");
		Serial.print(lat);
		if(east)
			Serial.print("E, ");
		else
			Serial.print("W, ");
		Serial.print(speed);
		Serial.print(", ");
		Serial.println(course);
	}
	else
		Serial.print('.');
}

void serialEvent() {
	Serial.print('.');
	while(Serial1.available()) {
		char ch = Serial1.read();
		if(ch == '\n') {
			message[ptr] = '\0';
			_locator->Parse(message);
			ptr = 0;
		}
		else
			message[ptr++] = ch;
	}
}