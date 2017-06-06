#include "RF24.h"
#include "HBridge.h"
#include <avr/wdt.h>

//pwm pins: 3, 5, 6, 9, 10, 11
//------------ 5 and 6 is low priority
//The frequency of the PWM signal on most pins is approximately 490 Hz
//On the Uno and similar boards, pins 5 and 6 have a frequency of approximately 980 Hz


//opts - em desenvolvimento para troca de dados
enum opts
{
	COUNTERCLOCKWISE = 0b0000,
	CLOCKWISE,
	MOTOR0 = 0b0000,
	MOTOR1 = 0b0010,
	MOTOR2,
	MOTOR3,
	ROBOT0 = 0b0000,
	ROBOT1 = 0b1000,
	ROBOT2,
	ROBOT3,
	ROBOT4,
	ROBOT5,
	ROBOT6,
	ROBOT7,
};

#define ROBOT_ID 0

struct command {
	unsigned char opt1;
	uint8_t opt2, opt3;
	bool dir;
};

command comm;
RF24 NRF(7, 8);
const uint64_t pipe[3] = { 0xE8E8F0F0E1LL,0xE8E8F0F0E10L,0xE8E8F0F0E0LL };
HBridge motor[2];
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	Serial.println("Receiver!");
	motor[0].Set(2, 4, 10);
	motor[1].Set(5, 6, 9);
	NRF.begin();
	//new code - setup
	NRF.setPALevel(RF24_PA_MAX);
	NRF.setDataRate(RF24_2MBPS);
	NRF.setChannel(124);
	NRF.setRetries(0, 10);
	//end of new code
	NRF.openReadingPipe(ROBOT_ID, pipe);
	NRF.startListening();
	Serial.println("Setup Ok");
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (NRF.available())
	{
		NRF.read(&comm, sizeof(comm));
		Serial.println("Received!");
		switch (comm.opt1)
		{
		case '+':
			motor[comm.opt2].SpeedAdd(comm.opt3, comm.dir);
			break;
		case '-':
			motor[comm.opt2].Break();
			break;
		case 's':
			motor[comm.opt2].SpeedSet(comm.opt3, comm.dir);
			break;
		default:
			break;
		}
	}
}

void On_Error(String message) {
	pinMode(10, OUTPUT);
	while (true) {
		if (Serial.available()) {
			switch (Serial.read()) {
			case 'p':
				Serial.println(message);
				Serial.println("'c' to continue, 'r' to reboot");
				while (!Serial.available());
				break;
			case 'c':
				return;
				break;
			case 'r':
				Reboot();
				break;
			default:
				break;
			}
		}
		digitalWrite(10, !digitalRead(10));
		delay(500);
	}
}

void Reboot()
{
	wdt_enable(WDTO_15MS);
	while (1)
	{
	}
}