#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "ntp.h"

//NtpClient ntpClient("pool.ntp.org", 30);
//NtpClient ntpClient("app.ido4pro.com", 30);
NtpClient ntpClient("51.254.36.188", 30);

void onPrintSystemTime()
{
	//m_printf("UTC Time: %s\n\r",SystemClock.getSystemTimeString(eTZ_UTC));

	/*_timeSystem._year = SystemClock.now(eTZ_UTC).Year;
	_timeSystem._month = SystemClock.now(eTZ_UTC).Month;
	_timeSystem._day = SystemClock.now(eTZ_UTC).Day;
	_timeSystem._dayWeek = _dayWeek(_timeSystem);

	_timeSystem._hour = SystemClock.now(eTZ_UTC).Hour;
	_timeSystem._minute = SystemClock.now(eTZ_UTC).Minute;
	_timeSystem._second = SystemClock.now(eTZ_UTC).Second;*/

	//SystemClock.now(eTZ_UTC).isNull();
}

void ntpClientRequest()
{
	ntpClient.setAutoQueryInterval(60);
	ntpClient.setAutoQuery(true);
	ntpClient.setAutoUpdateSystemClock(true);
	ntpClient.requestTime();
}

bool clockTimeOk()
{
	if(SystemClock.now(eTZ_UTC).isNull() == false
	&& SystemClock.now(eTZ_UTC).Year >= 2016)
		return true;
	else
		return false;
}
