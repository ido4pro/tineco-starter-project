#include "hw_ds1820.h"
#include "wdgHw.h"

irom cDs1820::cDs1820(uint8 num)
{
	_numDs = num;

	_tempRead._successDs1820 = false;
	_tempRead._tempDs1820 = 0;
}

void irom cDs1820::init()
{
	_tempRead._successDs1820 = false;
	_tempRead._tempDs1820 = 0;
}

void irom cDs1820::_readDs1820()
{
	uint8 a;
	uint64 info;
	
	_wdgHw._rearmWdgCounter(ID_DS1820_TIMER);

	if (!ReadTemp.MeasureStatus())  // the last measurement completed
	{
		if (ReadTemp.GetSensorsCount())   // is minimum 1 sensor detected ?
		{
			for(a=0;a<ReadTemp.GetSensorsCount();a++)   // prints for all sensors
			{
				if (ReadTemp.IsValidTemperature(a))   // temperature read correctly ?
				{
					int32 _temp = 0;
					_temp = ReadTemp.GetRaw(a);
					_temp *= 100;
					_temp /= 16;

					_tempRead._tempDs1820 = _temp;
					m_printf(" T%d = %d,%d Celsius\n\r",a+1,_tempRead._tempDs1820/100,_tempRead._tempDs1820%100);

					//Logger.info(" T%d = %d,%d Celsius\n\r",a+1,_ds1820Read._temperature/100,_ds1820Read._temperature%100);

					_tempRead._successDs1820 = true;
				}
				else
					m_printf("Temperature not valid");
			}
		}
		ReadTemp.StartMeasure();  // next measure, result after 1.2 seconds * number of sensors
	}
	else
		m_printf("No valid Measure so far! wait please");
}

void irom cDs1820::stop()
{
	_wdgHw._stopWdgCounter(ID_DS1820_TIMER);
	ds1820_timer.stop();

	_tempRead._successDs1820 = false;
	_tempRead._tempDs1820 = 0;
}


void irom cDs1820::start()
{
	ReadTemp.Init(12);  			// select PIN It's required for one-wire initialization!
	ReadTemp.StartMeasure(); // first measure start,result after 1.2 seconds * number of sensors

	ds1820_timer.initializeMs(DELAY_DS1820_TIMER, TimerDelegate(&cDs1820::_readDs1820, this)).start();   // every 20 seconds
	_wdgHw._startWdgCounter(ID_DS1820_TIMER);

	_tempRead._successDs1820 = false;
	_tempRead._tempDs1820 = 0;
}

cDs1820 ds1820(0);
