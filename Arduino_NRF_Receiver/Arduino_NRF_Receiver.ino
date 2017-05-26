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


//#include <RF24_config.h>
//#include <printf.h>
//#include <nRF24L01.h>
//#include "RF24.h"
//
//#include <RF24-master\RF24.h>
//
//RF24 NRF(9, 10);
//const uint64_t pipe = 0xE8E8F0F0E1LL;
//unsigned char opt;
//
//void setup() {
//	Serial.begin(9600);
//	Serial.write("Reader!");
//	NRF.begin();
//	NRF.openReadingPipe(1, pipe);
//	NRF.startListening();
//	Serial.write("setup ok!");
//}
//
//void loop() {
//	if (NRF.available()) {
//		Serial.println("receiving...");
//		NRF.read(&opt, sizeof(opt));
//		Serial.println("received... "+ opt);
//		Serial.println("Done!");
//	}
//}

//*
// Name:		Arduino_NRF_Receiver.ino
// Created:	5/23/2017 7:33:09 AM
// Author:	orley
//*/

//
////void loop() {
////	if (Serial.available()) {
////		opt1 = Serial.read();
////		opt2 = Serial.parseInt() - 1;
////		opt3 = Serial.parseInt();
////		opt4 = Serial.parseInt() - 1;
////		verify = Serial.read();
////		if (verify == '.') Serial.println(verify);
////		else Serial.println("error");
////		switch (opt1)
////		{
////		case '+':
////			motor[opt2].SpeedAdd(opt3, opt4);
////			break;
////		case '-':
////			motor[opt2].Break();
////			break;
////		case 's':
////			motor[opt2].SpeedSet(opt3, opt4);
////			break;
////		default:
////			break;
////		}
////	}
////}