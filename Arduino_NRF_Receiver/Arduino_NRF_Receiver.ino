#include "RF24.h"
#include "HBridge.h"

#define BOTID = 0;

//BOTID identifica o robo atual
//3 bits para identificar o robo
//1 bit para indicar direção

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

struct comando {
	unsigned char opt1;
	uint8_t opt2, opt3;
	bool dir;
};

comando comm;
RF24 NRF(7, 8);
const uint64_t pipe = 0xE8E8F0F0E1LL;
HBridge motor[2];	 
// the setup function runs once when you press reset or power the board
void setup() {
	motor[0].Set(2, 4, 10);
	motor[1].Set(5, 6, 9);
	Serial.begin(115200);
	Serial.println("Receiver!");
	NRF.begin();
	//new code - setup
	NRF.setPALevel(RF24_PA_MAX);
	NRF.setDataRate(RF24_2MBPS);
	NRF.setChannel(124);
	NRF.setRetries(0, 10);
	//end of new code
	NRF.openReadingPipe(1, pipe);
	NRF.startListening();
	Serial.println("Setup Ok");
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (NRF.available())
	{
		NRF.read(&comm, sizeof(comm));
		Serial.println("received!");
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


void error() {
	while (true) {
		digitalWrite(10, true);
		delay(100);
		digitalWrite(10, false);
		delay(100);
	}
}