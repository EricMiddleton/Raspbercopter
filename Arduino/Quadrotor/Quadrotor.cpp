#include "Quadrotor.h"

Quadrotor		*_quad;
Model			*_model;
unsigned long	_timerStartVal;
byte			count;

void Quadrotor::Init(int mass, int length, int thrust, int mAH) {
	Serial.begin(115200);
	Serial.println("Quadrotor Initialization Started!");

	//Initialize our global variables
	_quad = this;
	_model = &model;
	count = 0;

	//Initiate our member classes
	locator.Init();
	//communicator.Init();
	//battery.Init(mAH);
	model.Init(mass, thrust);


	//Let's update our quadrocopter
	//Independently of whatever the user
	//Puts in their loop(){} function!
	InitTimer();

	Serial.println("Quadrotor Initialization Complete!");
}

Vector Quadrotor::GetMOI() {
	return model.GetMOI();
}

void Quadrotor::SetMOI(Vector MOI) {
	model.SetMOI(MOI);
}

Angle Quadrotor::GetOrientation() {
	return model.GetOrientation();
}

float Quadrotor::GetAltitude() {
	return locator.GetAltitude();
}

Vector Quadrotor::GetPosition() {
	return locator.GetPosition();
}

Vector Quadrotor::GetVelocity() {
	return locator.GetVelocity();
}

float Quadrotor::GetGPSAccuracy() {
	return locator.GetGPSAccuracy();
}

/*
void Quadrotor::SendMessage(char *message) {
	communicator.SendMessage(message);
}

void Quadrotor::SendReceiveMessage(char *message, char *response) {
	communicator.SendReceiveMessage(message, response);
}

int Quadrotor::GetBatteryLevel() {
	return battery.GetLevel();
}

void Quadrotor::SetBatteryAlarmLevel() {
	battery.SetAlarmLevel();
}

boolean Quadrotor::GetBatteryAlarm() {
	return battery.GetAlarm();
}
*/

//This is the 10hz update method
//Any quadrotor methods that need to be
//Updated at this rate are called here
void Quadrotor::Update() {
	//TODO: Finish section
	return;
}

void Quadrotor::InitTimer() {
	//This value, combined with the 256 prescaler,
	//Will result in a 100hz update rate
	_timerStartVal = 64911;

	//Set prescaler to 256
	TCCR3A = 0;
	TCCR3B = 1<<CS12;

	//Enable the interrupt
	TIMSK3 = 1<<TOIE3;

	//Start the timer
	TCNT3 = _timerStartVal;
}

//Timer Interrupt
ISR(TIMER3_OVF_vect) {}/*
	//Disable this interrupt
	TIMSK3 = 0<<TOIE3;
	//Enable all other interrupts
	interrupts();

	//Update our quadrotor model
	//Every time the timer goes off
	_model->Update();
	
	//Update the rest of the quad 1/10 of the time
	if(!(count++ % 10))
		_quad->Update();
	
	if(count == 100) {
		//This will be the 1hz update section
		//Whenever I get to it
		count = 0;
	}

	//Reset the timer
	TCNT3 += _timerStartVal;

	//Re-enable the interrupt
	TIMSK3 = 1<<TOIE3;
}*/