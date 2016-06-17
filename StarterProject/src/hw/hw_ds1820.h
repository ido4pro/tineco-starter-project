#ifndef _DS1820_H_
#define _DS1820_H_

#include <Libraries/OneWire/OneWire.h>
#include <Libraries/DS18S20/ds18s20.h>

#include <SmingCore/SmingCore.h>
#include "type.h"

#ifndef _readDs1820
#define DELAY_DS1820_TIMER			20000 	// milliseconds
#endif

typedef struct {

	sint16 _tempDs1820;
	bool _successDs1820;

	sint16 _humiAm2320;
	sint16 _tempAm2320;
	bool _successAm2320;

} s_TempRead;

class cDs1820
{
private:

	uint8	_numDs;

public:

	cDs1820(uint8 num);
	
	s_TempRead _tempRead;
	
	DS18S20 ReadTemp;
	Timer 	ds1820_timer;

	void _tick_ds1820();

	void _readDs1820();
	
	void init();

	void start();
	void stop();
};


extern cDs1820 ds1820;

#endif
