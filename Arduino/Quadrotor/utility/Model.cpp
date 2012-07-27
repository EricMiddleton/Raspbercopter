#pragma once
#include "Model.h"
#include "Memory.h"

/*------------------------------
-----------Class PID------------
------------------------------*/
void PID::Init(float p, float i, float d) {
	this->p = p;
	this->i = i;
	this->d = d;

	error = dError = sError = 0;
}

void PID::SetPID(float p, float i, float d) {
	this->p = p;
	this->i = i;
	this->d = d;
}

Vector PID::GetPID() {
	return Vector(p, i, d);
}

float PID::Update(float wanted, float current) {
	float newError = wanted - current;

	dError = (newError - error)*0.01;

	//This isn't the 'correct' way to integrate the error,
	//But this way it will limit itself, and also go back to zero faster
	//To avoid some 'interesting' side effects
	sError += newError*0.01 - sError*0.001;

	error = newError;

	return p*error + i*sError + d*dError;
}

/*------------------------------
----------Class Model-----------
--------------------------------
-A physics model of a Quadrotor-
-That contains and controls the-
----Motors, IMU and PID loops---
------------------------------*/
void Model::Init(unsigned int mass, unsigned int thrust) {
	angle = Angle(0, 0, 0);
	this->mass = mass;
	this->thrust = thrust;
	multiplier = 1000/thrust;

	motor1.attach(PINBASE, 1000, 2000);
	motor2.attach(PINBASE + 1, 1000, 2000);
	motor3.attach(PINBASE + 2, 1000, 2000);
	motor4.attach(PINBASE + 3, 1000, 2000);
	
	motor1.writeMicroseconds(2000);
	motor2.writeMicroseconds(2000);
	motor3.writeMicroseconds(2000);
	motor4.writeMicroseconds(2000);

	Serial.println(F("Connect Motors"));
	while(!Serial.available());

	motor1.writeMicroseconds(1000);
	motor2.writeMicroseconds(1000);
	motor3.writeMicroseconds(1000);
	motor4.writeMicroseconds(1000);

	Serial.println(F("Motor Init Complete."));

	imu.Init();
	Serial.println(F("IMU Init Complete."));

	//Pitch, Yaw, and Roll PID controllers
	pid1.Init(3 * mass, 0.0 * mass, 1 * mass);
	pid2.Init(3 * mass, 0.0 * mass, 1 * mass);
	pid3.Init(3 * mass, 0.0 * mass, 1 * mass);

	//Height PID controller
	pid4.Init(3 * mass, 0.0 * mass, 1 * mass);

	altimeter.Init();
}

Angle Model::GetAngle() {
	return angle;
}

Angle Model::GetOrientation() {
	return imu.GetOrientation();
}

void Model::SetAngle(Angle a) {
	angle = a;
}

void Model::Update() {
	Angle curAng, force;
	float motorOut1, motorOut2, altPid;

	altimeter.UpdateRanger();
	imu.Update();

	curAng = imu.GetOrientation();
	force = Angle(
				pid1.Update(angle.GetPitch(), curAng.GetPitch()),
				pid2.Update(angle.GetYaw(), curAng.GetYaw()),
				pid3.Update(angle.GetRoll(), curAng.GetRoll()));
	altPid = 0;//pid4.Update(height, altimeter.GetRangerAltitude(curAng));
				
	//Quadrotor has cross-type layout
	//So each motor affects both pitch and roll
	//motor[0] is at front-right and motor[3] is bottom-right
	motorOut1 = force.GetPitch() + force.GetRoll();
	motorOut2 = force.GetPitch() - force.GetRoll();

	/*
	motor1.writeMicroseconds(max(0, motorOut1 + altPid)*multiplier + 1000);
	motor2.writeMicroseconds(max(0, motorOut2 + altPid)*multiplier + 1000);
	motor3.writeMicroseconds(max(0, -motorOut1 + altPid)*multiplier + 1000);
	motor4.writeMicroseconds(max(0, -motorOut2 + altPid)*multiplier + 1000);*/

}