#ifndef _MAINPROCESS_H_
#define _MAINPROCESS_H_

#define DELAY_MAINPROCESS_TIMER		1000 	// milliseconds

#define _STOP						-1

#include <SmingCore/SmingCore.h>

#include "type.h"

typedef enum {
	_MP_init = 0,
	_MP_waitConnect = 1,
	_MP_waitNtpTime = 2,
	_MP_requetteHTTP = 3,
	_MP_waitMqttConnect = 4,
	_MP_verifSendFileData = 5,
	_MP_allConnect = 6,
} e_statMainProcess;

class cMainP
{
private:

	uint8 numMainP;

	int32 _cntMainProcess;
	e_statMainProcess _statMainProcess;
	
	uint32			_countSecond;
	int32 			_smartWifiInProcess;
	
	Timer mainProcess_timer;

public:

	cMainP(uint8 num);

	void	_tick_mainP();
	void 	_sendHttpRequest();
	uint8 	_httpReqRes;

	void _start();
	void _stop();
};

extern HttpClient httpClient;
extern void		_httpRequestResponse(HttpClient& client, bool successful);

extern void		tick_mainP();

extern cMainP mainP;

#endif
