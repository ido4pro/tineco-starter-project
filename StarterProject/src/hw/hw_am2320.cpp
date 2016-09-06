#include <SmingCore/SmingCore.h>
#include "hw_am2320.h"
#include "wdgHw.h"
#include "hw_gpio.h"

irom _cAm2320::_cAm2320(uint8 num)
{
	numAm2320 = num;

	for(_nbBit = 0;_nbBit < 50;_nbBit++)
		_bit[_nbBit] = 0;
	_nbBit = 0;

	_Humidity = 0;
	_Temperature = 0;

	_hHumidity = 0;
	_lHumidity = 0;
	_hTemperature = 0;
	_lTemperature = 0;
	_parity = 0;
}

void irom _cAm2320::_readAm2320()
{
	uint8 _rd;
	uint8 _rd_ = 0;

	uint32 _idx = 0;
	uint32 _cnt_0 = 0;
	uint32 _cnt_1 = 0;

	_wdgHw._rearmWdgCounter(ID_AM2320_TIMER);

	for(_nbBit = 0;_nbBit < 50;_nbBit++)
		_bit[_nbBit] = 0;
	_nbBit = 0;

	int32 _count;

	// start signal
	_gpioAm2320._outputSet(_OFF);
	os_delay_us(1000);
	_gpioAm2320._outputSet(_ON);
	_gpioAm2320._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);// SDA

	// wait falling
	_count = 0;
	while(_gpioAm2320._inputGet() > 0)
	{
		_count++;
		if(_count > TIME_OUT_COUNT)
		{
			m_printf("am: time out falling\n\r");
			return;
		}
	}

	// wait LOW time start
	_count = 0;
	while(_gpioAm2320._inputGet() == 0)
	{
		_count++;
		if(_count > TIME_OUT_COUNT)
		{
			m_printf("am: time out wait LOW\n\r");
			return;
		}
	}

	// wait HIGH time start
	_count = 0;
	while(_gpioAm2320._inputGet() > 0)
	{
		_count++;
		if(_count > TIME_OUT_COUNT)
		{
			m_printf("am: time out wait HIGH\n\r");
			return;
		}
	}

	// start recept
	do
	{
		_rd = _gpioAm2320._inputGet();

//		if(_gpio12._outputGet() == _ON)
//			_gpio12._outputSet(_OFF);
//		else
//			_gpio12._outputSet(_ON);

		if(_rd > 0)
			_cnt_1 ++;
		else
			_cnt_0 ++;

		if(_cnt_1 > 60)
		{
			_idx = 100;
		}

		if(_rd_ == 1 && _rd == 0)
		{
			if(_cnt_1 >= _cnt_0)
				_bit[_nbBit++] = 1;
			else
				_bit[_nbBit++] = 0;

			_cnt_0 = 0;
			_cnt_1 = 0;
			_idx ++;
		}

		_rd_ = _rd;
	}
	while(_idx < 50);

//	_gpio12._outputSet(_OFF);
	_gpioAm2320._conf(_OUTPUT,_PULL_NONE,_ON,_INTR_DISABLE);// SDA

/*	m_printf("bit %d:",_nbBit);
	for(_idx = 0;_idx < _nbBit;_idx++)
		m_printf("%d",_bit[_idx]);
	m_printf("\n\r");*/

	if(_nbBit == 40)
	{
		_rd = 0;
		_idx = 0;
		_hHumidity = 0;
		if(_bit[_idx++] == 1) _hHumidity |= 0x80;
		if(_bit[_idx++] == 1) _hHumidity |= 0x40;
		if(_bit[_idx++] == 1) _hHumidity |= 0x20;
		if(_bit[_idx++] == 1) _hHumidity |= 0x10;
		if(_bit[_idx++] == 1) _hHumidity |= 0x08;
		if(_bit[_idx++] == 1) _hHumidity |= 0x04;
		if(_bit[_idx++] == 1) _hHumidity |= 0x02;
		if(_bit[_idx++] == 1) _hHumidity |= 0x01;
		_rd += _hHumidity;

		_lHumidity = 0;
		if(_bit[_idx++] == 1) _lHumidity |= 0x80;
		if(_bit[_idx++] == 1) _lHumidity |= 0x40;
		if(_bit[_idx++] == 1) _lHumidity |= 0x20;
		if(_bit[_idx++] == 1) _lHumidity |= 0x10;
		if(_bit[_idx++] == 1) _lHumidity |= 0x08;
		if(_bit[_idx++] == 1) _lHumidity |= 0x04;
		if(_bit[_idx++] == 1) _lHumidity |= 0x02;
		if(_bit[_idx++] == 1) _lHumidity |= 0x01;
		_rd += _lHumidity;

		_hTemperature = 0;
		if(_bit[_idx++] == 1) _hTemperature |= 0x80;
		if(_bit[_idx++] == 1) _hTemperature |= 0x40;
		if(_bit[_idx++] == 1) _hTemperature |= 0x20;
		if(_bit[_idx++] == 1) _hTemperature |= 0x10;
		if(_bit[_idx++] == 1) _hTemperature |= 0x08;
		if(_bit[_idx++] == 1) _hTemperature |= 0x04;
		if(_bit[_idx++] == 1) _hTemperature |= 0x02;
		if(_bit[_idx++] == 1) _hTemperature |= 0x01;
		_rd += _hTemperature;

		_lTemperature = 0;
		if(_bit[_idx++] == 1) _lTemperature |= 0x80;
		if(_bit[_idx++] == 1) _lTemperature |= 0x40;
		if(_bit[_idx++] == 1) _lTemperature |= 0x20;
		if(_bit[_idx++] == 1) _lTemperature |= 0x10;
		if(_bit[_idx++] == 1) _lTemperature |= 0x08;
		if(_bit[_idx++] == 1) _lTemperature |= 0x04;
		if(_bit[_idx++] == 1) _lTemperature |= 0x02;
		if(_bit[_idx++] == 1) _lTemperature |= 0x01;
		_rd += _lTemperature;

		_parity = 0;
		if(_bit[_idx++] == 1) _parity |= 0x80;
		if(_bit[_idx++] == 1) _parity |= 0x40;
		if(_bit[_idx++] == 1) _parity |= 0x20;
		if(_bit[_idx++] == 1) _parity |= 0x10;
		if(_bit[_idx++] == 1) _parity |= 0x08;
		if(_bit[_idx++] == 1) _parity |= 0x04;
		if(_bit[_idx++] == 1) _parity |= 0x02;
		if(_bit[_idx++] == 1) _parity |= 0x01;


/*		m_printf("Hex %02X %02X %02X %02X %02X/%02X\n\r",	_hHumidity,
														_lHumidity,
														_hTemperature,
														_lTemperature,
														_parity,
														_rd
														);
														*/
		if(_rd == _parity)
		{
			_Humidity = (_hHumidity << 8) | _lHumidity;
			_Temperature = (_hTemperature << 8) | _lTemperature;

			_tempRead._tempAm2320 = _Temperature * 10;
			_tempRead._humiAm2320 = _Humidity * 10;

			_tempRead._successAm2320 = true;

			m_printf("am: %d %d\n\r",_tempRead._humiAm2320,_tempRead._tempAm2320);
		}
		else
			m_printf("am: _parity\n\r");
	}
	else
		m_printf("am: _nbBit\n\r");

}

void irom _cAm2320::_start()
{
	_gpioAm2320._conf(_OUTPUT,_PULL_NONE,_ON,_INTR_DISABLE);// SDA

	_tempRead._successAm2320 = false;
	_tempRead._tempAm2320 = 0;
	_tempRead._humiAm2320 = 0;

	am2320_timer.initializeMs(DELAY_AM2320_TIMER, TimerDelegate(&_cAm2320::_readAm2320, this)).start();   // every 20 seconds
	_wdgHw._startWdgCounter(ID_AM2320_TIMER);
}

void irom _cAm2320::_stop()
{
	_wdgHw._stopWdgCounter(ID_AM2320_TIMER);
	am2320_timer.stop();

	_tempRead._successAm2320 = false;
	_tempRead._tempAm2320 = 0;
	_tempRead._humiAm2320 = 0;
}

_cAm2320 _am2320(0);
