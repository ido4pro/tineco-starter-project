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

	bool _statEtalon;
	int32 _refEtalon;

} s_etalonADC;

typedef struct {
	// for read current Level
	int32 _currentLevel;
	int32 _wattPerHour;

} s_adcRead;

extern s_etalonADC		_etalonADC;
extern s_adcRead		_adcRead;

class _cAdc
{
private:

	uint8	_numAdc;

	int8  	_statAdc;
	int8  	_numberAdc;
	int32 	_currentLevel;

	int32 	_readCurrent[ADC_NUMBER_READ];

	int32 	_readCurrentLevel();

	int32 	_readADC();
	
	Timer   adc_timer;
	
	void 	_tick_adc();
	
public:

	_cAdc(uint8 num);
	
	void 	_start();
	void 	_stop();
};


extern _cAdc		_adc;

#endif
