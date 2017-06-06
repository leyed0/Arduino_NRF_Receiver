// 
// 
// 

#include "HBridge.h"

void HBridge::init()
{


}

//for the encoder the code gona use 2 more pins per motor
//Set the pins used by the motor. p1, p2 = input pins acording to the direction. p3 = Speed Pin(pwm)
//total: 5 pins per motor, 4 general and 1 PWM
void HBridge::Set(int p1, int p2, int sp3) {
	Pin[0] = p1;
	Pin[1] = p2;
	SpdPin = sp3;
	pinMode(Pin[0], OUTPUT);
	pinMode(Pin[1], OUTPUT);
	pinMode(SpdPin, OUTPUT);
}

//Set the speed (-255 to 255 -> pwm)
void HBridge::SpeedSet(int speed, bool dir) {
	Speed = speed;
	Direction = dir;
	if (Speed > 255) Speed = 255;

	digitalWrite(Pin[0], Direction);
	digitalWrite(Pin[1], !Direction);
	analogWrite(SpdPin, Speed);
}

//Add a speed to the actual.
void HBridge::SpeedAdd(int speed, bool dir = true) {
	if (dir = Direction) Speed += speed;
	else if ((Speed -= speed) < 0) Direction = !Direction;
	SpeedSet(abs(Speed), Direction);
}

void HBridge::Break() {
	Speed = 0;
	digitalWrite(Pin[0], true);
	digitalWrite(Pin[1], true);
	analogWrite(SpdPin, 0);
}

void HBridge::Kill() {
	Speed = 0;
	digitalWrite(Pin[0], false);
	digitalWrite(Pin[1], false);
	digitalWrite(SpdPin, false);
}

int HBridge::SpeedGet() {
	return Speed;
}
int HBridge::SpeedGets() {
	return analogRead(SpdPin);
}

int HBridge::DirGet() {
	return Direction;
}

//HBridge HBridge;

