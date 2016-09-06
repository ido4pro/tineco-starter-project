#ifndef WDGHW_H_
#define WDGHW_H_

#define _gpioWdg _gpio16

// watchdog hardware management
#define NBR_TIMER_MAX				20

#define DELAY_WDG_TIMER 			500 	// milliseconds

#define WD_STOP						-1

#define ID_MAINPROCESS_TIMER		0
#define ID_TELEINFO_TIMER			1
#define ID_DS1820_TIMER				2
#define ID_SENDDATA_TIMER			3
#define ID_ADC_TIMER				4
#define ID_RELAY_TIMER				5
#define ID_AM2320_TIMER				6


#include "type.h"


typedef struct {

	int32 _cntWdg;

} s_wdgTimer;


class _cWdgHw
{
private:

	uint8 numWdgHw;

	void _increaseWdgCounter();
	
	Timer wdgHw_timer;

public:

	_cWdgHw(uint8 num);

	void _tick_wdgHw();
	
	int32			_nbrWdgTimer;
	s_wdgTimer		_wdgTimer[NBR_TIMER_MAX];

	void _rearmWdgCounter(uint8 id);
	void _startWdgCounter(uint8 id);
	void _stopWdgCounter(uint8 id);
	
	void _init();

	void _start();
	void _stop();
};





extern _cWdgHw _wdgHw;

#endif
