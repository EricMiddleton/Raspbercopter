#pragma once
#ifndef SENSORS_H
#define SENSORS_H

#include "Datatypes.h"

class Sensor
{
public:
					Sensor(byte address = 0);
	virtual void	Update() = 0;
	virtual void	Get(void *out);
protected:
	Triplet data;
	byte address;
};

class Accelerometer : Sensor
{
public:
			Accelerometer(byte address = 0);
	void	Update();
	void	Get(Vector *out);
};

class Gyroscope : Sensor
{
public:
			Gyroscope(byte address = 0);
	void	Update();
	void	Get(Angle *out);
private:
	void	WriteRegister(byte addr, byte value);
	byte	ReadRegister(byte addr);
	void	ReadRegister(byte addr, byte count, byte *out);
};
/*
class Mangetometer : Sensor
{
public:
			Magnetometer();
			Magnetometer(byte address);
	void	Update();
	void	Get(Vector *out);
};

class Barometer : Sensor
{
public:
			Barometer(byte address);
	void	Update();
	void	Get(float *out);
};
*/

class Ranger : Sensor
{
public:
							Ranger();
	void					Update();
	void					Get(float *out);
private:
	void					InterruptHandler(unsigned long time, boolean status);
	static void				Interrupt();
	unsigned long volatile	upTime;
};

//Necessary for the ranger ISR
extern Ranger	*_ranger;

#endif