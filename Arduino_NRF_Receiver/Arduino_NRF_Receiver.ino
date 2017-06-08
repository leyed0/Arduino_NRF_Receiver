#include "RF24.h"
#include "HBridge.h"
#include <avr/wdt.h>

//pwm pins: 3, 5, 6, 9, 10, 11
//------------ 5 and 6 is low priority
//The frequency of the PWM signal on most pins is approximately 490 Hz
//On the Uno and similar boards, pins 5 and 6 have a frequency of approximately 980 Hz

#define ROBOT_ID 1

struct MotorCmd {
	uint8_t speed[2];
	bool direction[2];
};

RF24 NRF(7, 8);
const uint64_t pipe[3] = { 0xE8E8F0F0E1LL,0xE8E8F0F0E10L,0xE8E8F0F0E0LL };

HBridge motor[2];
String msg;
MotorCmd MtCmd;

unsigned char CMDID;
// the setup function runs once when you press reset or power the board
void setup() {
	wdt_disable();
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
	NRF.openReadingPipe(1, pipe[ROBOT_ID]);
	NRF.startListening();
	Serial.println("Setup Ok");
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (NRF.available())
	{
		NRF.read(&CMDID, sizeof(CMDID));
		Serial.print("Receiveing ");
		Serial.print(char(CMDID));
		Serial.println(" command");


		while (!NRF.available());

		switch (CMDID)
		{
		case 'M':
			NRF.read(&MtCmd, sizeof(MtCmd));
			Serial.println(MtCmd.speed[0]);
			Serial.println(MtCmd.direction[0]);
			Serial.println(MtCmd.speed[1]);
			Serial.println(MtCmd.direction[1]);

			if (motor[0].SpeedGet() != MtCmd.speed[0] || motor[0].DirGet() != MtCmd.direction[0]) motor[0].SpeedSet(MtCmd.speed[0], MtCmd.direction[0]);
			if (motor[1].SpeedGet() != MtCmd.speed[1] || motor[1].DirGet() != MtCmd.direction[1]) motor[1].SpeedSet(MtCmd.speed[1], MtCmd.direction[1]);
			break;
		case 'R':
			Reboot();
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