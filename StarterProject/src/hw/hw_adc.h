#ifndef ADC_INCLUDE__ADC_H_
#define ADC_INCLUDE__ADC_H_

#define ADC_NUMBER_READ				10

#define ADC_BIT_MAX					1024
#define ADC_VOLT_MAX				1000 	// 1000 mv

#define RATIO_DP_CURRENT 			51 		// 5,1 * 10

#define ACS712_SENSITIVITY			100		// asc712-20A 100 mv/A

#define DISCHARGE_TIME_RSTPEAK		25000 	// 5RC

#define DELAY_ADC_TIMER 			100 	// milliseconds

#include <SmingCore/SmingCore.h>

#include "type.h"



typedef struct {
	// for read current Level


} s_adcRead;

class _cAdc
{
private:

	uint8	_numAdc;

	int8  	_statAdc;
	int8  	_numberAdc;

	int32 	_readCurrent[ADC_NUMBER_READ];

	int32 	_readCurrentLevel();

	int32 	_readADC();

	void	_resetPeak();
	
	Timer   adc_timer;
	
	void 	_tick_adc();
	
public:

	_cAdc(uint8 num);
	
	int32 _currentLevel;
	int32 _wattPerHour;
	
	
	void 	_start();
	void 	_stop();
};


extern _cAdc		_adc;

#endif
