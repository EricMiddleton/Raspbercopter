#include "Sensors.h"
#include <Wire.h>

Sensor::Sensor(byte address) {
	this->address = address;
}

void Sensor::Get(void *out) {
	((Triplet*)out)->SetA(data.GetA());
	((Triplet*)out)->SetB(data.GetB());
	((Triplet*)out)->SetB(data.GetC());
}

void Sensor::Update() {
}

//Accelerometer: Wii Nunchuck
//Modify these methods to use a different
//3-axis accelerometer
Accelerometer::Accelerometer(byte address) : Sensor(address) {
}

void Accelerometer::Init() {
	//Initialize the nunchuck
	Wire.beginTransmission(address);
	Wire.write(0xF0);
	Wire.write(0x55);
	Wire.endTransmission();

	delay(1);

	Wire.beginTransmission(address);
	Wire.write(0xFB);
	Wire.write(0x00);
	Wire.endTransmission();

	Update();
}

void Accelerometer::Update() {
	byte in[6], ptr = 0;

	Wire.requestFrom(address, (byte)6);

	while(Wire.available() && ptr < 6)
		in[ptr++] = Wire.read();

	if(ptr == 6) {
		data.SetA( ((in[3] << 2) + ((in[5] & 0x30) >> 4)) - 512 );
		data.SetB( ((in[2] << 2) + ((in[5] & 0x0C) >> 2)) - 512 );
		data.SetC( ((in[4] << 2) + ((in[5] & 0xC0) >> 6)) - 512 );
	}

	Wire.beginTransmission(address);
	Wire.write(0x00);
	Wire.endTransmission();
}

void Accelerometer::Get(Vector *out) {
	out->SetX(data.GetA());
	out->SetY(data.GetB());
	out->SetZ(data.GetC());
}

//Gyroscope: ITG 3205
//Modify these methods to use a different
//3-axis accelerometer

Gyroscope::Gyroscope(byte address) : Sensor(address) {
}

void Gyroscope::Init() {
	//Disable Sample-rate division
	WriteRegister(0x15, 0);

	//Set +/-2000 deg/sec range with 256hz filter
	WriteRegister(0x16, 0x18 );

	//Enable the fresh data interrupt
	WriteRegister(0x17, 0x15);

	//Disable sleep mode and enable internal oscillator
	WriteRegister(0x3E, 0);

	//Wait for everything to initialize properly
	delay(70);

	Update();
}

void Gyroscope::Update() {
	byte in[6];
	int rawX, rawY, rawZ;
		
	//Read Raw 16-bit Gyroscope values
	ReadRegister(0x1D, 6, in);
	rawX = (ReadRegister(0x1D) << 8) | ReadRegister(0x1E);
	rawY = (ReadRegister(0x1F) << 8) | ReadRegister(0x20);
	rawZ = (ReadRegister(0x21) << 8) | ReadRegister(0x22);

	//Convert the values into proper orientation
	//Radian values. Magic value '0.0005326403454625252390092899270584f'
	//is 2000 deg/s / 0xFFFF * PI / 180
	data.SetA(rawX * 0.0005326403454625252390092899270584f);
	data.SetB(-rawZ * 0.0005326403454625252390092899270584f);
	data.SetC(rawY * 0.0005326403454625252390092899270584f);
}

void Gyroscope::Get(Angle *out) {
	out->SetPitch(data.GetA());
	out->SetYaw(data.GetB());
	out->SetRoll(data.GetC());
}

void Gyroscope::WriteRegister(byte addr, byte value) {
	//Start transmission with gyroscope
	Wire.beginTransmission(address);

	//Send the gyroscope the register address
	//And value to write
	Wire.write(addr);
	Wire.write(value);

	//End the transmission
	Wire.endTransmission();
}

byte Gyroscope::ReadRegister(byte addr) {
	unsigned long start = micros(), end = start + 1000;
	byte data;

	Wire.beginTransmission(address);
	Wire.write(addr);
	Wire.endTransmission();

	Wire.beginTransmission(address);
	Wire.requestFrom((byte)address, (byte)1);

	while(!Wire.available() && (end > micros()));

	data = Wire.read();
	Wire.endTransmission();

	return data;
}

void Gyroscope::ReadRegister(byte addr, byte count, byte *out) {
	byte ptr = 0;

	Wire.beginTransmission(address);
	Wire.write(addr);
	Wire.endTransmission();

	Wire.beginTransmission(address);
	Wire.requestFrom(address, count);

	while(Wire.available())
		out[ptr++] = Wire.read();

	Wire.endTransmission();
}

//Magnetometer: HMC5883L
//Modify these methods to use a different
//3-axis magnetometer

//TODO:
//EVERYTHING!

//Barometer: BMP085
//Modify these methods to use a different
//Barometer

//TODO:
//EVERYTHING!

//Ultrasonic Ranger: HC-SR04
//Modify these methods to use a different
//Ultrasonic Ranger

Ranger		*_ranger;

Ranger::Ranger() : Sensor() {
}

void Ranger::Init() {
	//Initialize the global variable
	_ranger = this;

	//We're only using the first float in the triplet
	//Probably not the best way to do it
	//I'm open to suggestions
	data.SetA(0);

	upTime = 0;

	//Pin 2 is trigger, pin 3 is out
	pinMode(2, OUTPUT);
	digitalWrite(2, LOW);
	pinMode(3, INPUT);

	Update();
}

void Ranger::Update() {
	//Initialize an input-change interrupt on
	//Interrupt 1 (pin 3)
	attachInterrupt(1, Ranger::Interrupt, CHANGE);

	//Trigger the ultrasonic sensor
	digitalWrite(2, HIGH);
	delayMicroseconds(10);
	digitalWrite(2, LOW);
}

void Ranger::Get(float *out) {
	*out = data.GetA();
}

void Ranger::Interrupt() {
	detachInterrupt(1);
	unsigned long time = micros();
	
	//Enable other interrupts
	interrupts();

	_ranger->InterruptHandler(time, digitalRead(3) == HIGH);
}

void Ranger::InterruptHandler(unsigned long time, boolean status) {
	if(status) {
		upTime = time;
		attachInterrupt(1, Ranger::Interrupt, CHANGE);
	}
	else {
		float range;
		if(time > upTime) {
			time -= upTime;
			if(time >= 38000)
				range = -1;
			else
				//Magic number is conversion factor
				//from microseconds to meters with
				//speed of sound ~340m/s
				range = (float)time*0.0001724137931;
		}
		data.SetA(range);
	}
}