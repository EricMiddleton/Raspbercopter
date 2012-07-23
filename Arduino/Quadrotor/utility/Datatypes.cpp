#pragma once
#ifndef DATATYPES_CPP
#define DATATYPES_CPP
#include "Datatypes.h"


Triplet::Triplet(float a, float b, float c) {
	this->a = a;
	this->b = b;
	this->c = c;
}

Triplet::Triplet() {
	this->a = this->b = this->c = 0;
}

void Triplet::SetA(float a) {
	this->a = a;
}

void Triplet::SetB(float b) {
	this->b = b;
}

void Triplet::SetC(float c) {
	this->c = c;
}

float Triplet::GetA() {
	return a;
}

float Triplet::GetB() {
	return b;
}

float Triplet::GetC() {
	return c;
}

void Triplet::Print() {
	Serial.print("(");
	Serial.print(a);
	Serial.print(", ");
	Serial.print(b);
	Serial.print(", ");
	Serial.print(c);
	Serial.println(")");
}



Vector::Vector() : Triplet() {
};

Vector::Vector(float x, float y, float z) : Triplet(a, b, c) {
}

void Vector::SetX(float x) {
	a = x;
}
void Vector::SetY(float y) {
	b = y;
}
void Vector::SetZ(float z) {
	c = z;
}
float Vector::GetX() {
	return a;
}
float Vector::GetY() {
	return b;
}
float Vector::GetZ() {
	return c;
}

Angle Vector::ToAngle(byte ref) {
	if(ref == GRAVITY)
		return Angle(atan2(c, a), 0, atan2(c, b));
	else
		return Angle(0, 0, 0);
}



Angle::Angle() : Triplet() {
};

Angle::Angle(float pitch, float yaw, float roll) : Triplet(pitch, yaw, roll) {
}

void Angle::SetPitch(float pitch) {
	this->a = pitch;
}

void Angle::SetYaw(float yaw) {
	this->b = yaw;
}

void Angle::SetRoll(float roll) {
	this->c = roll;
}

float Angle::GetPitch() {
	return a;
}

float Angle::GetYaw() {
	return b;
}

float Angle::GetRoll() {
	return c;
}

Angle Angle::operator+(Angle ang) {
	return Angle(a + ang.GetPitch(), b + ang.GetYaw(), c + ang.GetRoll());
}

Angle Angle::operator*(Angle ang) {
	return Angle(a * ang.GetPitch(), b * ang.GetYaw(), c * ang.GetRoll());
}

Angle Angle::operator*(float f) {
	return Angle(a * f, b * f, c * f);
}

void Angle::wrap()
{
	if(a > PI)
		a -= PI2;
	else if(a < -PI)
	  a += PI2;

	if(b > PI)
		b -= PI2;
	else if(b < -PI)
		b += PI2;

	if(c > PI)
		c -= PI2;
	else if(c < -PI)
		c += PI2;
}

void Angle::Print() {
	Serial.print("(");
	Serial.print(a*180/PI);
	Serial.print(", ");
	Serial.print(b*180/PI);
	Serial.print(", ");
	Serial.print(c*180/PI);
	Serial.println(")");
}

#endif