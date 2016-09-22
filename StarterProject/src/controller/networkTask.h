#ifndef _NETWORKTASK_H_
#define _NETWORKTASK_H_

#define DELAY_NET_TIMER		1000 	// milliseconds

#define _STOP						-1

#include <SmingCore/SmingCore.h>

#include "type.h"

typedef enum {
	NET_INIT = 0,
	NET_GOT_IP = 1,
	NET_WAIT_NTP_TIME = 2,
	NET_EXECUTE_POST = 3,
	NET_WAIT_MQTT_CONNECT = 4,
	NET_ALL_CONNECT = 5,
} e_statNetworkTask;

class NetworkTask
{
private:

	uint8 numMainP;

	int32 _cntNetworkTask;
	e_statNetworkTask _statNetworkTask;
	
	uint32			_countSecond;
	int32 			smartWifiInProcess;
	
	Timer netTimer;
	
	void	tickNetworkTask();

public:

	NetworkTask(uint8 num);
	
	HttpClient httpClient;

	
	void 	_sendHttpRequest();
	uint8 	_httpReqRes;

	void start();
	void stop();
};

//extern HttpClient httpClient;
//extern void		_httpRequestResponse(HttpClient& client, bool successful);

//extern void		tick_mainP();

extern NetworkTask networkTask;

#endif
