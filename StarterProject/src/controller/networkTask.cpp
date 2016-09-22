#include "networkTask.h"
#include "version.h"

#include <SmingCore/SmingCore.h>
#include "define.h"

#include "mqttServer.h"

#include "ntp.h"
#include "util.h"
#include "timeUtil.h"

#include "wdgHw.h"

#include "logger.h"
#include "command.h"


#include "smartwifi.h"
#include "webserver.h"


void irom _httpRequestResponse(HttpClient& client, bool successful)
{
	if (!successful)
	{
	 	os_printf("start request fail!\n");
	 	networkTask._httpReqRes = 1;
	 	return;
	}
	else
	{
		String response = client.getResponseString();
		
		//String response = DEVICE_ID;

		os_printf("start request done : %s\n", response.c_str());

		Logger.setName(response);

		startMqttEsp((char*) response.c_str());

		networkTask._httpReqRes = 2;
	}
	
	
}

void irom NetworkTask::_sendHttpRequest()
{
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	char buf1[50];
	char buf2[50];

	sprintf(buf1, "%s", WifiStation.getIP().toString().c_str());
	sprintf(buf2, "%s", WifiStation.getMAC().c_str());

	root["ip"] = buf1;
	root["type"] = "esp12";
	root["fonc"] = "thermostat";

	root["id1"] = system_get_chip_id();
	root["id2"] = spi_flash_get_id();
	root["mac"] = buf2;
	
	root["version"] = VERSION;

	root["slot"]= rboot_get_current_rom();


	//Logger.info("%s", root.toJsonString().c_str());
	//os_printf("%s", root.toJsonString().c_str());

	this->httpClient.setRequestContentType("application/json");
	
	String tempString;
	root.printTo(tempString);

	httpClient.setPostBody(tempString);
	httpClient.downloadString(START_URL, _httpRequestResponse);
}

void irom _connectOk()
{
	debugf("I'm CONNECTED\n");
	
	ShowInfo();
}


irom NetworkTask::NetworkTask(uint8 num)
{
	numMainP = num;

	_cntNetworkTask = _STOP;
	_statNetworkTask = NET_INIT;

	_httpReqRes = 0;
	
	_countSecond = 0;
	
	smartWifiInProcess = _STOP;
}

void irom NetworkTask::tickNetworkTask()
{
	_wdgHw._rearmWdgCounter(ID_NET_TIMER);
	
	if(_cntNetworkTask >= 0)
		_cntNetworkTask++;

	switch(_statNetworkTask)
	{
	case NET_INIT:
		debugf("NET_INIT\n\r");

		debugf("UTC Time: %s\n\r",SystemClock.getSystemTimeString(eTZ_UTC).c_str());
		stopMqttEsp();

		_cntNetworkTask = 0;
		_statNetworkTask = NET_GOT_IP;
		break;
	case NET_GOT_IP:
		debugf("NET_GOT_IP\n\r");
		if(wifi_station_get_connect_status() == STATION_GOT_IP)
		{
			startWebServer();
			ntpClientRequest();

			_cntNetworkTask = 0;
			_statNetworkTask = NET_WAIT_NTP_TIME;
		}
		break;
	case NET_WAIT_NTP_TIME:
		debugf("NET_WAIT_NTP_TIME\n\r");

		debugf("UTC Time: %s\n\r",SystemClock.getSystemTimeString(eTZ_UTC).c_str());


		if(clockTimeOk() == true)
		{
			_cntNetworkTask = 0;
			_statNetworkTask = NET_EXECUTE_POST;
		}
		if(wifi_station_get_connect_status() != STATION_GOT_IP)
		{
			debugf("lost wifi connection\n\r");
			_statNetworkTask = NET_INIT;
		}
		break;
	case NET_EXECUTE_POST:
		debugf("NET_EXECUTE_POST\n\r");
		if(_cntNetworkTask == 1)
		{
			_httpReqRes = 0;
			_sendHttpRequest();
		}
		else
		{
			if(_httpReqRes == 1)// start request fail
			{
				_cntNetworkTask = 0;
			}
			if(_httpReqRes == 2)// start request sucess
			{
				_cntNetworkTask = 0;
				_statNetworkTask = NET_WAIT_MQTT_CONNECT;
			}
		}
		if(_cntNetworkTask >= 30)
		{
			debugf("time out requette http\n\r");
			_statNetworkTask = NET_INIT;
		}
		if(wifi_station_get_connect_status() != STATION_GOT_IP)
		{
			debugf("lost wifi connection\n\r");
			_statNetworkTask = NET_INIT;
		}
		break;
	case NET_WAIT_MQTT_CONNECT:
		if(_cntNetworkTask == 1)
			debugf("NET_WAIT_MQTT_CONNECT\n\r");

		if(_cntNetworkTask >= 10)
		{
			debugf("timeOut mqtt connection\n\r");
			_statNetworkTask = NET_INIT;
		}

		if(isMqttEspReady() == true)
		{
			debugf("mqtt connection\n\r");
			_cntNetworkTask = 0;
			_statNetworkTask = NET_ALL_CONNECT;
		}

		if(wifi_station_get_connect_status() != STATION_GOT_IP)
		{
			debugf("lost wifi connection\n\r");
			_statNetworkTask = NET_INIT;
		}
		break;

	case NET_ALL_CONNECT:
		if(_cntNetworkTask == 1)
		{
			debugf("NET_ALL_CONNECT\n\r");
		
		}

		if(wifi_station_get_connect_status() != STATION_GOT_IP)
		{
			debugf("lost wifi connection\n\r");
			_statNetworkTask = NET_INIT;
		}
		else
		if(isMqttEspReady() == false)
		{
			debugf("lost mqtt connection\n\r");
			_statNetworkTask = NET_INIT;
		}
		break;
	}

	if(smartWifiInProcess >= 0)
	{
		smartWifiInProcess++;
		if(smartWifiInProcess >= 30)
		{
			smartWifiInProcess = _STOP;

			debugf("stopSmartWifi\n\r");

			stopSmartWifi();

			WifiAccessPoint.enable(false);
			WifiStation.enable(false);
			WifiStation.disconnect();

			os_delay_us(500000);

			WifiAccessPoint.enable(true);
			WifiStation.enable(true);

			WifiStation.config(WifiStation.getSSID(), WifiStation.getPassword());
			WifiStation.waitConnection(_connectOk);

			_cntNetworkTask = 0;
			_statNetworkTask = NET_INIT;
		}
	}

	if((_countSecond % 60) == 0)
		Logger.info("_countSecond : %d",_countSecond);

	_countSecond++;
}


void irom NetworkTask::start()
{
	_cntNetworkTask = 0;
	_statNetworkTask = NET_INIT;

	//_sdata._init();

	debugf("START_NETWORK_TASK\n\r");

	netTimer.initializeMs(DELAY_NET_TIMER, TimerDelegate(&NetworkTask::tickNetworkTask, this)).start();
	_wdgHw._startWdgCounter(ID_NET_TIMER);

    ETS_GPIO_INTR_ENABLE();
}

void irom NetworkTask::stop()
{
	ETS_GPIO_INTR_DISABLE();

	_cntNetworkTask = _STOP;

	_wdgHw._stopWdgCounter(ID_NET_TIMER);
	netTimer.stop();

	stopMqttEsp();

}

NetworkTask networkTask(0);

