#include "user_config.h"
#include <SmingCore/SmingCore.h>

#include "wdgHw.h"

#include "hw_gpio.h"


irom _cWdgHw::_cWdgHw(uint8 num)
{
	numWdgHw = num;
}

void irom _cWdgHw::_init()
{
	for(_nbrWdgTimer = 0;_nbrWdgTimer < NBR_TIMER_MAX;_nbrWdgTimer++)
		_wdgTimer[_nbrWdgTimer]._cntWdg = WD_STOP;
	_nbrWdgTimer = 0;
}


void irom _cWdgHw::_tick_wdgHw()
{
	_gpioWdg._outputSet(_OFF);
	os_delay_us(6000);
	_gpioWdg._outputSet(_ON);

	_increaseWdgCounter();
}

void irom _cWdgHw::_increaseWdgCounter()
{
	int i;
	int timeOut = 100000;
	for(i = 0;i < NBR_TIMER_MAX;i++)
	{
		if(	_wdgTimer[i]._cntWdg >= 0)
			_wdgTimer[i]._cntWdg ++;

		switch(i)
		{
		case ID_NET_TIMER:	timeOut = 8;		break;
		case ID_DS1820_TIMER:		timeOut = 80;		break;
		case ID_ADC_TIMER:			timeOut = 4;		break;
		case ID_SENDDATA_TIMER:		timeOut = 4;		break;
		case ID_RELAY_TIMER:		timeOut = 2;		break;
		default: 					timeOut = 100000;	break;
		}

		if(_wdgTimer[i]._cntWdg >= timeOut)
		{
			//debugf("\r\nERROR TIMER STOP %d ... RESART SYSTEM\r\n",i);
			_stop();
		}
	}
}

void irom _cWdgHw::_rearmWdgCounter(uint8 id)
{
	_wdgTimer[id]._cntWdg = 0;
}

void irom _cWdgHw::_startWdgCounter(uint8 id)
{
	_wdgTimer[id]._cntWdg = 0;
}

void irom _cWdgHw::_stopWdgCounter(uint8 id)
{
	_wdgTimer[id]._cntWdg = WD_STOP;
}

void irom _cWdgHw::start()
{
	_tick_wdgHw();
	wdgHw_timer.initializeMs(DELAY_WDG_TIMER, TimerDelegate(&_cWdgHw::_tick_wdgHw, this)).start();
}

void irom _cWdgHw::_stop()
{
	wdgHw_timer.stop();
}

_cWdgHw _wdgHw(0);
