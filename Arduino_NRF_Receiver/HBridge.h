// HBridge.h

#ifndef _HBRIDGE_h
#define _HBRIDGE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class HBridge
{
private:
	int Pin[2], SpdPin, Speed = 0;
	bool Direction = false;
 protected:

 public:
	void Set(int, int, int);
	void SpeedSet(int,bool);
	void SpeedAdd(int,bool);
	int SpeedGet();
	int SpeedGets();
	int DirGet();
	void Break();
	void Kill();
	void init();
};

//extern HBridge HBridge;

#endif

