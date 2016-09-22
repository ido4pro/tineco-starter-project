#ifndef _AM2320_H
#define _AM2320_H

#include "type.h"
#include "hw_gpio.h"

#define DELAY_AM2320_TIMER			20000 	// milliseconds

#define TIME_OUT_COUNT	100

typedef struct {

	sint16 _humiAm2320;
	sint16 _tempAm2320;
	bool _successAm2320;

} s_TempRead2320;

class _cAm2320
{
private:

	uint8 numAm2320;


	uint8 _nbBit;
	uint8 _bit[50];

	uint8 _hHumidity;
	uint8 _lHumidity;
	uint8 _hTemperature;
	uint8 _lTemperature;
	uint8 _parity;


public:

	_cAm2320(uint8 num);

	short _Humidity;
	short _Temperature;

	Timer 	am2320_timer;

	s_TempRead2320 _tempRead;

	void _readAm2320();

	void _start();
	void _stop();
};

extern _cAm2320 _am2320;

#endif
