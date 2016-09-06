#include <SmingCore/SmingCore.h>

#include "hw_relay.h"

#include "wdgHw.h"
#include "hw_gpio.h"


irom _cRelay::_cRelay(uint8 num)
{
	numRelay = num;
	_stat = _OFF;
}

void irom _cRelay::_tick_relay()
{
	_wdgHw._rearmWdgCounter(ID_RELAY_TIMER);
	_changeStat();

	debugf("tick relay\n\r");
}

void irom _cRelay::_changeStat()
{
	_gpioDtcNull._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);// DTC_NULL (stop)
	_wdgHw._stopWdgCounter(ID_RELAY_TIMER);
	relay_timer.stop();

    if(_stat == _ON)
    	_gpioRelay._outputSet(_ON);
	else
		_gpioRelay._outputSet(_OFF);
}

void irom _cRelay::_On()
{
	if(_stat == _OFF)
	{
		relay_timer.initializeMs(DELAY_RELAY_TIMER, TimerDelegate(&_cRelay::_tick_relay, this)).start();
		_wdgHw._startWdgCounter(ID_RELAY_TIMER);

		_stat = _ON;

		_gpioDtcNull._conf(_INTERRUPT,_PULL_NONE,_OFF,_INTR_ANYEDGE);// DTC_MOVE (start)
	}
}

void irom _cRelay::_Off()
{
	if(_stat == _ON)
	{
		relay_timer.initializeMs(DELAY_RELAY_TIMER, TimerDelegate(&_cRelay::_tick_relay, this)).start();
		_wdgHw._startWdgCounter(ID_RELAY_TIMER);

		_stat = _OFF;

		_gpioDtcNull._conf(_INTERRUPT,_PULL_NONE,_OFF,_INTR_ANYEDGE);// DTC_MOVE (start)
	}
}

_GPIO_OutputStat irom _cRelay::_statu()
{
	return _gpioRelay._outputGet();
}


_cRelay _relay(0);
