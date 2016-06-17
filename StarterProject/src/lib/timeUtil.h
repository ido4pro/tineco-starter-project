#ifndef _TIME_UTIL_H
#define _TIME_UTIL_H

#include "type.h"

typedef struct {
	uint16	_year;
	uint8	_month;
	uint8	_day;
	uint8	_dayWeek;

/*	switch(_dayWeek)
	{
		case 1 :"Sunday"
		case 2 :"Monday"
		case 3 :"Tuesday"
		case 4 :"Wednesday"
		case 5 :"Thursday"
		case 6 :"Friday"
		case 7 :"Saturday"
	}*/

	uint16	_hour;
	uint8	_minute;
	uint8	_second;

} s_timeUtil;

extern void				_initTime				(s_timeUtil *time);

extern uint16			_numberOfDays			(s_timeUtil time);
extern uint8			_maxDaysInMouth			(s_timeUtil time);
extern uint8			_dayWeek				(s_timeUtil time);

extern s_timeUtil		_convertSecondInDate	(uint32 _sec);
extern uint32			_convertDateInSecond	(s_timeUtil time);

extern char*			_formatTimeNowToPrint	(char* time);
extern char*			_formatTimeUtilToPrint	(s_timeUtil _Time,char* time);

extern s_timeUtil		_setSystemTime			();

#endif
