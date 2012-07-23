#pragma once
#ifndef DATATYPES_H
#define DATATYPES_H

#define PI		3.141592654
#define PI2		6.283185308

#define			GRAVITY		0
#define			NORTH		1

#define	Sign(n)	(((n) > 0) ? (1) : (0))
#include <Arduino.h>

class Triplet
{
public:
	Triplet();
	Triplet(float a, float b, float c);
	void           SetA(float a);
	void           SetB(float b);
	void           SetC(float c);
	float          GetA();
	float          GetB();
	float          GetC();

	void           Print();
protected:
	float a, b, c;
};

class Angle : Triplet
{
public:
	Angle			();
  Angle          (float pitch, float yaw, float roll);
  void           SetPitch(float pitch);
  void           SetYaw(float yaw);
  void           SetRoll(float roll);
  float          GetPitch();
  float          GetYaw();
  float          GetRoll();
  Angle          operator+(Angle);
  Angle          operator*(Angle);
  Angle          operator*(float);
  void           wrap();
  void				Print();
};

class Vector : Triplet
{
public:
	Vector			();
  Vector		 (float x, float y, float z);
  void           SetX(float x);
  void           SetY(float y);
  void           SetZ(float z);
  float          GetX();
  float          GetY();
  float          GetZ();

  Angle			ToAngle(byte ref);
};

#endif