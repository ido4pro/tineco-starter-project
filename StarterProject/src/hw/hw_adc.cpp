
#include <SmingCore/SmingCore.h>

#include "hw_adc.h"
#include "hw_gpio.h"
#include "wdgHw.h"


irom _cAdc::_cAdc(uint8 num)
{
	_numAdc = num;

	_statAdc = 0;
	_numberAdc = 0;
	_currentLevel = 0;
}

void irom _cAdc::_tick_adc()
{
	_wdgHw._rearmWdgCounter(ID_ADC_TIMER);
	
	int32 i = 0;
	int32 _calc = 0;

	_readCurrent[_numberAdc] = _readCurrentLevel();
	_resetPeak();

	//m_printf(": %d\n\r",_readCurrent[_numberAdc]);

	_numberAdc++;
	_statAdc++;
	if(_numberAdc >= ADC_NUMBER_READ)
	{
		_currentLevel = 0;

		for(i = 0;i < ADC_NUMBER_READ;i++)
		{
			_currentLevel += _readCurrent[i];
		}

		_currentLevel = _currentLevel / ADC_NUMBER_READ;
		if(_currentLevel > 0)
			_wattPerHour = _currentLevel * 220 * 1000 / 1414;
		else
			_wattPerHour = 0;

		_statAdc = 0;
		_numberAdc = 0;
	}
}

void irom _cAdc::_start()
{
	_statAdc = 0;
	_numberAdc = 0;

	adc_timer.initializeMs(DELAY_ADC_TIMER, TimerDelegate(&_cAdc::_tick_adc, this)).start();
	_wdgHw._startWdgCounter(ID_ADC_TIMER);
}

void irom _cAdc::_stop()
{
	_wdgHw._stopWdgCounter(ID_ADC_TIMER);
	adc_timer.stop();

	_statAdc = 0;
	_numberAdc = 0;
}

int32 irom _cAdc::_readCurrentLevel()
{
	int32 value = 0;

    _gpio4._outputSet(_OFF);
	os_delay_us(100);
	value = _readADC();

	value *= RATIO_DP_CURRENT;
	value /= 10;

	value /= ACS712_SENSITIVITY;
	value /= 1000;

	if(value > 2)
		return value;
	else
		return 0;
}

void irom _cAdc::_resetPeak()
{
    _gpio14._outputSet(_ON);
	os_delay_us(DISCHARGE_TIME_RSTPEAK);
    _gpio14._outputSet(_OFF);
}

int32 irom _cAdc::_readADC()
{
	int32 value = 0;

	value = system_adc_read();

	//debugf("cr:%d\n\r",value);

	value = (value * ADC_VOLT_MAX * 1000);
	value = (value / ADC_BIT_MAX);

	return value;// In mV
}

_cAdc _adc(0);


