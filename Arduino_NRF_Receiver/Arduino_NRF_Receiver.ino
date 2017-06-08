#include "RF24.h"
#include "HBridge.h"


struct comando {
	unsigned char opt1, verify;
	uint8_t opt2, opt3;
	bool dir;
};

comando comm;
RF24 NRF(7, 8);
byte addresses[][6] = { "1Node","2Node" };
const uint64_t pipe = 0xE8E8F0F0E1LL;
HBridge motor[2];
// the setup function runs once when you press reset or power the board
void setup() {
	motor[0].Set(2, 4, 10);
	motor[1].Set(5, 6, 9);
	Serial.begin(9600);
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
		Serial.write("receiving!\n");
		NRF.read(&comm, sizeof(comm));
		Serial.println("Received:");
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
		Serial.println(comm.opt1);
		Serial.println(comm.opt2);
		Serial.println(comm.opt3);
		Serial.println(comm.dir);
	}
}