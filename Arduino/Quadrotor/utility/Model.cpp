#pragma once
#include "Model.h"

extern "C" {
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
}

/*------------------------------
-----------Class PID------------
------------------------------*/
void PID::Init(float p, float i, float d) {
	this->p = p;
	this->i = i;
	this->d = d;

	error = dError = sError = 0;
	lastTime = 0;
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

	dError = (newError - error)*0.001;
	sError += newError*0.001 - sError*0.01;

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
void Model::Init(float mass, unsigned int thrust) {
	angle = Angle(0, 0, 0);
	height = 1.;

	this->mass = mass;

	motor1.Init(0, thrust);
	motor2.Init(1, thrust);
	motor3.Init(2, thrust);
	motor4.Init(3, thrust);
	
	motor1.SetThrust(thrust);
	motor2.SetThrust(thrust);
	motor3.SetThrust(thrust);
	motor4.SetThrust(thrust);

	Serial.println("Enter any key to finish setting esc bounds!");
	while(!Serial.available());
	
	motor1.SetThrust(0);
	motor2.SetThrust(0);
	motor3.SetThrust(0);
	motor4.SetThrust(0);


	imu.Init();

	//Pitch, Yaw, and Roll PID controllers
	pid1.Init(50, 0.0, 45);
	pid2.Init(50, 0.0, 45);
	pid3.Init(50, 0.0, 45);

	//Height PID controller
	pid4.Init(400, 0.0, 250);

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

unsigned int thrust = 0;
void Model::Update() {
	altimeter.UpdateRanger();

	if(Serial.available()) {
		char ch = Serial.read() - 48;
		thrust += 50*((ch == 6) - (ch == 4));
		thrust *= !!ch;
		if(thrust > 10000)
			thrust = 0;
		else if(thrust > 750)
			thrust = 0;

		motor1.SetThrust(thrust);
		motor2.SetThrust(thrust);
		motor3.SetThrust(thrust);
		motor4.SetThrust(thrust);
	}
	Angle curAng, force;
	float motorOut1, motorOut2, motorOut3, motorOut4, altPid, alt;

	imu.Update();

	curAng = imu.GetOrientation();
	alt = altimeter.GetRangerAltitude(curAng);

	force = Angle(
				pid1.Update(angle.GetPitch(), curAng.GetPitch()),
				pid2.Update(angle.GetYaw(), curAng.GetYaw()),
				pid3.Update(angle.GetRoll(), curAng.GetRoll()))*50;

	altPid = 0;//pid4.Update(height, alt);
				
	//Quadrotor has cross-type layout
	//So each motor affects both pitch and roll
	//motor[0] is at front-left and motor[3] is bottom-left
	motorOut1 = force.GetPitch() + force.GetRoll() + altPid;
	motorOut2 = force.GetPitch() - force.GetRoll() + altPid;
	//Let's exploit some symmetry!
	motorOut3 = -motorOut1;
	motorOut4 = -motorOut2;

	count++;
	if(count == 25) {
/*
		Serial.print(motorOut1);
		Serial.print("      ");
		Serial.print(motorOut2);
		Serial.print("      ");
		Serial.print(motorOut3);
		Serial.print("      ");
		Serial.print(motorOut4);
		Serial.print("      ");
		Serial.println(alt);
*/
		curAng.Print();

		count = 0;
	}/*
	motor1.SetThrust(max(0, motorOut1));
	motor2.SetThrust(max(0, motorOut2));
	motor3.SetThrust(max(0, motorOut3));
	motor4.SetThrust(max(0, motorOut4));*/

}