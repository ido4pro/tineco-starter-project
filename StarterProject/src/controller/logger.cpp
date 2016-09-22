#include "logger.h"
#include "mqttServer.h"
#include "timeUtil.h"

LoggerClass::LoggerClass()
{
}

LoggerClass::~LoggerClass()
{
}

void LoggerClass::emit(char* topic, char* data)
{
	mqttEspClient.publishWithQoS(topic, data, strlen(data), 0, true);
}


void LoggerClass::status()
{

	char topic[30];
	sprintf(topic, "%s/os", Logger.loggerName.c_str());

	
	char time[100];
	char buf[256];

	debugf(".");

	sprintf(buf, "{\"name\":\"%s\",\"time\":\"%s\",\"freemem\":%d,\"rssi\":%d}",
			Logger.loggerName.c_str(),
			_formatTimeNowToPrint(time),
			system_get_free_heap_size(),
			wifi_station_get_rssi()
			);

	this->emit(topic,buf);

}

void LoggerClass::setLevel(log_type level)
{
	this->currentLevel = level;
}

void LoggerClass::setName(String name)
{
	this->loggerName = name;

	statusTimer = new Timer();

	statusTimer->initializeMs(30000, TimerDelegate(&LoggerClass::status, this)).start();
}

String LoggerClass::getName()
{
	return this->loggerName;
}

void LoggerClass::debug(log_type level, const char *fmt, va_list args)
{

	int size = strlen(fmt);
	char topic[30];
	char buf[256];

	if (size < 128)
	{
		
		sprintf(topic, "%s/log", Logger.loggerName.c_str());
		
	

		char* p = buf;

		strcpy(buf, "{");

		sprintf(buf, "{\"name\":\"%s\",\"level\":%d,\"msg\":\"",
				this->loggerName.c_str(), (int) level);

		int currentLen = strlen(buf);

		m_vsnprintf(buf + currentLen, sizeof(buf) - currentLen, fmt, args);
		
		debugf("%s",buf + currentLen);

	}
	
	if(_allConnected() == true)
	{
		strcat(buf + strlen(buf), "\"}");
		this->emit(topic,buf);
	}

}

void LoggerClass::trace(const char *fmt, ...)
{
	if (currentLevel <= LOG_TYPE_TRACE)
	{
		va_list args;

		va_start(args, fmt);

		this->debug(LOG_TYPE_TRACE, fmt, args);

		va_end(args);
	}
}



void LoggerClass::info(const char *fmt, ...)
{
	if (currentLevel <= LOG_TYPE_INFO)
	{
		va_list args;

		va_start(args, fmt);

		this->debug(LOG_TYPE_INFO, fmt, args);

		va_end(args);
	}
}

void LoggerClass::warm(const char *fmt, ...)
{
	if (currentLevel <= LOG_TYPE_WARM)
	{
		va_list args;

		va_start(args, fmt);

		this->debug(LOG_TYPE_WARM, fmt, args);

		va_end(args);
	}
}

void LoggerClass::error(const char *fmt, ...)
{
	if (currentLevel <= LOG_TYPE_ERROR)
	{
		va_list args;

		va_start(args, fmt);

		this->debug(LOG_TYPE_ERROR, fmt, args);

		va_end(args);
	}
}

void LoggerClass::fatal(const char *fmt, ...)
{
	if (currentLevel <= LOG_TYPE_FATAL)
	{
		va_list args;

		va_start(args, fmt);

		this->debug(LOG_TYPE_FATAL, fmt, args);

		va_end(args);
	}
}

LoggerClass Logger;
