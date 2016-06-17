#ifndef RELAY_INCLUDE__RELAY_H_
#define RELAY_INCLUDE__RELAY_H_

#define DELAY_RELAY_TIMER			100 	// milliseconds

#include <SmingCore/SmingCore.h>

#include "type.h"
#include "hw_gpio.h"

class _cRelay
{
private:

	uint8 numRelay;

	_GPIO_OutputStat _stat;
	
	Timer 	relay_timer;

public:

	_cRelay(uint8 num);

	void _tick_relay();

	void _On();
	void _Off();
	void _Switch();
	
	bool _status();

	void _changeStat();
};

extern _cRelay _relay;

#endif
