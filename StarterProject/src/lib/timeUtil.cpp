#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "timeUtil.h"

void irom _initTime(s_timeUtil *time)
{
	time->_year = 0;
	time->_month = 0;
	time->_day = 0;
	time->_dayWeek = 0;
	time->_hour = 0;
	time->_minute = 0;
	time->_second = 0;
}

uint16 irom _numberOfDays(s_timeUtil time)
{
	sint16 _nbrOfYears				=  time._year - 1990;
	sint16 _nbrOfLeapYears			= (time._year - 1989) / 4;
	uint32 _nbrOfDays1				= (_nbrOfYears * 365) + _nbrOfLeapYears;
	uint32 _nbrOfDays2				= (time._month - 1) * 30;

	if(time._month < 9)
		_nbrOfDays2 += time._month / 2;
	else
		_nbrOfDays2 += (time._month + 1) / 2;

	if(time._month > 2)
	{
		if(!(time._year & 3))
			_nbrOfDays2 -= 1;
		else
			_nbrOfDays2 -= 2;
	}

	return (uint16)(_nbrOfDays1 + _nbrOfDays2 + time._day - 1);
}

uint8 irom _maxDaysInMouth (s_timeUtil time)
{
	uint8 _nbrOfDays = 30;

	if(time._month < 8)
		_nbrOfDays +=  time._month	  & 1;
	else
		_nbrOfDays += (time._month + 1) & 1;

	if(time._month == 2)
	{
		if(!(time._year & 3))
			_nbrOfDays -= 1;
		else
			_nbrOfDays -= 2;
	}

	return _nbrOfDays;
}

uint8 irom _dayWeek (s_timeUtil time)
{
	uint16 	_nbrOfDays		= _numberOfDays(time);
	uint8 	_nbrOfWeeks	 	= _nbrOfDays / 7;
	time._dayWeek			= _nbrOfDays - (_nbrOfDays * 7) + 2;

	if(time._dayWeek > 7)
		time._dayWeek -= 7;

	return  time._dayWeek;
}

s_timeUtil irom _convertSecondInDate	(uint32 _sec)
{
	s_timeUtil time;

	time._day		= 1;
	time._month		= 1;
	time._year		= 1970;

	time._hour	= 0;
	time._minute	= 0;
	time._second	= 0;

	uint32 _refIncr;

	//--- increase year ---//
	_refIncr = (365*24*3600);
	while(_sec >= _refIncr)
	{
		time._year++;
		_sec -= _refIncr;
		if(!(time._year & 3))
			_refIncr = (366*24*3600);
		else
			_refIncr = (365*24*3600);
	}

	//--- increase month ---//
	_refIncr = _maxDaysInMouth(time)*24*3600;
	while(_sec >= _refIncr)
	{
		time._month++;
		_sec -= _refIncr;
		_refIncr = _maxDaysInMouth(time)*24*3600;
	}

	//--- increase day ---//
	_refIncr = 24*3600;
	while(_sec >= _refIncr)
	{
		time._day++;
		_sec -= _refIncr;
	}

	//--- increase hour ---//
	_refIncr = 3600;
	while(_sec >= _refIncr)
	{
		time._hour++;
		_sec -= _refIncr;
	}

	//--- increase minute ---//
	_refIncr = 60;
	while(_sec >= _refIncr)
	{
		time._minute++;
		_sec -= _refIncr;
	}

	//--- increase second ---//
	_refIncr = 1;
	while(_sec >= _refIncr)
	{
		time._second++;
		_sec -= _refIncr;
	}

	return time;
}

uint32 irom _convertDateInSecond(s_timeUtil time)
{
	uint32 _dateInSecond;

	sint16 _nbrOfYears				=  time._year - 1970;
	sint16 _nbrOfLeapYears			= (time._year - 1969) / 4;
	uint32 _nbrOfDays1				= (_nbrOfYears * 365) + _nbrOfLeapYears;
	uint32 _nbrOfDays2				= (time._month - 1) * 30;

	if(time._month < 9)
		_nbrOfDays2 += time._month / 2;
	else
		_nbrOfDays2 += (time._month + 1) / 2;

	if(time._month > 2)
	{
		if(!(time._year & 3))
			_nbrOfDays2 -= 1;
		else
			_nbrOfDays2 -= 2;
	}

	_dateInSecond = (_nbrOfDays1 + _nbrOfDays2 + time._day - 1) * 3600 * 24;

	_dateInSecond += (time._hour * 3600);
	_dateInSecond += (time._minute * 60);
	_dateInSecond += (time._second);

	return _dateInSecond;
}

s_timeUtil irom _setSystemTime()
{
	s_timeUtil time;

	time._year = SystemClock.now(eTZ_UTC).Year;
	time._month = SystemClock.now(eTZ_UTC).Month+1;
	time._day = SystemClock.now(eTZ_UTC).Day;
	time._hour = SystemClock.now(eTZ_UTC).Hour;
	time._minute = SystemClock.now(eTZ_UTC).Minute;
	time._second = SystemClock.now(eTZ_UTC).Second;

	return time;
}

char* irom	_formatTimeNowToPrint(char* time)
{
	sprintf(time, "%04d-%02d-%02dT%02d:%02d:%02d.000Z"
			,SystemClock.now(eTZ_UTC).Year
			,SystemClock.now(eTZ_UTC).Month+1
			,SystemClock.now(eTZ_UTC).Day
			,SystemClock.now(eTZ_UTC).Hour
			,SystemClock.now(eTZ_UTC).Minute
			,SystemClock.now(eTZ_UTC).Second
			);

	return time;
}

char* irom _formatTimeUtilToPrint(s_timeUtil _Time,char* time)
{
	sprintf(time, "%04d-%02d-%02dT%02d:%02d:%02d.000Z"
			,_Time._year
			,_Time._month
			,_Time._day
			,_Time._hour
			,_Time._minute
			,_Time._second
			);



	return time;
}
