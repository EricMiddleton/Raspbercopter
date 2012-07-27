#include "Quadrotor.h"
#include "utility/Memory.h"

Quadrotor		*_quad;
Model			*_model;
unsigned int	_timerStartVal;
byte			_count;

Quadrotor::Quadrotor() {
}

void Quadrotor::Init(unsigned int mass, byte length, unsigned int thrust, unsigned int mAH) {
	Serial.begin(115200);
	Serial.println(F("Initialization Started."));

	//Initialize our global variables
	_quad = this;
	_model = &model;
	_count = 0;

	//Initiate our member classes
	//locator.Init();
	//communicator.Init();
	//battery.Init(mAH);
	model.Init(mass, thrust);
	Serial.println(F("Model Init Complete."));


	//Let's update our quadrocopter
	//Independently of whatever the user
	//Puts in their loop(){} function!
	InitTimer();
	Serial.println(F("Timer Init Complete."));

	Serial.println(F("\tInitialized."));
}

Angle Quadrotor::GetOrientation() {
	return model.GetOrientation();
}
/*
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
*/
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
ISR(TIMER3_OVF_vect) {
	TIMSK3 = 0;
	interrupts();

	//Update our quadrotor model
	//Every time the timer goes off
	_model->Update();
	
	//Update the rest of the quad 1/10 of the time
	if(!(_count++ % 10)) {
		_quad->Update();
	}
	
	if(_count == 100) {
		//TODO:
		//This will be the 1hz update section
		//Whenever I get to it
		_quad->GetOrientation().Print();
		_count = 0;
	}

	//Reset the timer
	TCNT3 += _timerStartVal;

	//Re-enable the interrupt
	TIMSK3 = 1<<TOIE3;
}