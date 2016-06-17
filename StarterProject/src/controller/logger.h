#ifndef INCLUDE_LOGGER_H_
#define INCLUDE_LOGGER_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>

/*
 The level descriptions are best practice opinions.

 "fatal" (60): The service/app is going to stop or become unusable now. An operator should definitely look into this soon.
 "error" (50): Fatal for a particular request, but the service/app continues servicing other requests. An operator should look at this soon(ish).
 "warn" (40): A note on something that should probably be looked at by an operator eventually.
 "info" (30): Detail on regular operation.
 "debug" (20): Anything else, i.e. too verbose to be included in "info" level.
 "trace" (10): Logging from external libraries used by your app or very detailed application logging.
 */

typedef enum {
	LOG_TYPE_TRACE = 10,
	LOG_TYPE_DEBUG = 20,
	LOG_TYPE_INFO = 30,
	LOG_TYPE_WARM = 40,
	LOG_TYPE_ERROR = 50,
	LOG_TYPE_FATAL = 60
} log_type;

class LoggerClass {
public:
	LoggerClass();
	virtual ~LoggerClass();

	String loggerName = "nameNotSet/log";
	void setLevel(log_type level);
	
	void setName(String name);
	String getName();

	void resolveDns(String urlServer);

	void trace(const char *fmt, ...);
	void debug(const char *fmt, ...);
	void info(const char *fmt, ...);
	void warm(const char *fmt, ...);
	void error(const char *fmt, ...);
	void fatal(const char *fmt, ...);
	



private:

	log_type currentLevel = LOG_TYPE_TRACE;

	Timer* statusTimer;
	

	void debug(log_type level, const char *fmt, va_list args);

	void emit(char* topic, char* data);
	
	void status();	
	

};

extern LoggerClass Logger;

#endif /* INCLUDE_LOGGER_H_ */
