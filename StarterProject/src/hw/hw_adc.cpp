
#include <SmingCore/SmingCore.h>

#include "hw_adc.h"
#include "hw_gpio.h"
#include "wdgHw.h"

s_etalonADC		_etalonADC;
s_adcRead		_adcRead;

irom _cAdc::_cAdc(uint8 num)
{
	_numAdc = num;

	_statAdc = 0;
	_numberAdc = 0;
	_currentLevel = 0;

	_etalonADC._statEtalon = false;
	_etalonADC._refEtalon = 0;
}

void irom _cAdc::_tick_adc()
{
	_wdgHw._rearmWdgCounter(ID_ADC_TIMER);
	
	int32 i = 0;
	int32 _calc = 0;

	_readCurrent[_numberAdc] = _readCurrentLevel();

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

		if(_currentLevel > 0)
			_currentLevel = (_currentLevel / ADC_NUMBER_READ);
		else
			_currentLevel = 0;

		if(_etalonADC._statEtalon == false)
		{
			_etalonADC._refEtalon = _currentLevel;
			_etalonADC._statEtalon = true;
		}
		else
		{
			if(_currentLevel > _etalonADC._refEtalon)
				_adcRead._currentLevel = (_currentLevel - _etalonADC._refEtalon);
			else
				_adcRead._currentLevel = 0;


			if(_adcRead._currentLevel > 0)
				_adcRead._wattPerHour = _adcRead._currentLevel * 220 * 1000 / 1414;
			else
				_adcRead._wattPerHour = 0;
		}

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

int32 irom _cAdc::_readADC()
{
	int32 value = 0;

	value = system_adc_read();

	value = (value * ADC_VOLT_MAX * 1000);
	value = (value / ADC_BIT_MAX);

	return value;// In mV
}

_cAdc _adc(0);


