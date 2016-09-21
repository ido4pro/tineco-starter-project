#include "mainProcess.h"
#include "version.h"

#include <SmingCore/SmingCore.h>

#include "hw_adc.h"

#include "hw_ds1820.h"
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

HttpClient httpClient;

void irom _httpRequestResponse(HttpClient& client, bool successful)
{
	if (!successful)
	{
	 	os_printf("start request fail!\n");
	 	mainP._httpReqRes = 1;
	 	return;
	}
	else
	{
		String response = client.getResponseString();
		
		//String response = DEVICE_ID;

		os_printf("start request done : %s\n", response.c_str());

		Logger.setName(response);

		startMqttEsp((char*) response.c_str());

		mainP._httpReqRes = 2;
	}
	
	
}

void irom cMainP::_sendHttpRequest()
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

	httpClient.setRequestContentType("application/json");
	
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


irom cMainP::cMainP(uint8 num)
{
	numMainP = num;

	_cntMainProcess = _STOP;
	_statMainProcess = _MP_init;

	_httpReqRes = 0;
	
	_countSecond = 0;
	
	_smartWifiInProcess = _STOP;
}

void irom cMainP::_tick_mainP()
{
	_wdgHw._rearmWdgCounter(ID_MAINPROCESS_TIMER);
	
	if(_cntMainProcess >= 0)
		_cntMainProcess++;

	switch(_statMainProcess)
	{
	case _MP_init:
		debugf("_MP_init\n\r");

		debugf("UTC Time: %s\n\r",SystemClock.getSystemTimeString(eTZ_UTC).c_str());
		stopMqttEsp();

		_cntMainProcess = 0;
		_statMainProcess = _MP_waitConnect;
		break;
	case _MP_waitConnect:
		debugf("_MP_waitConnect\n\r");
		if(wifi_station_get_connect_status() == STATION_GOT_IP)
		{
			startWebServer();
			ntpClientRequest();

			_cntMainProcess = 0;
			_statMainProcess = _MP_waitNtpTime;
		}
		break;
	case _MP_waitNtpTime:
		debugf("_MP_waitNtpTime\n\r");

		debugf("UTC Time: %s\n\r",SystemClock.getSystemTimeString(eTZ_UTC).c_str());


		if(clockTimeOk() == true)
		{
			_cntMainProcess = 0;
			_statMainProcess = _MP_requetteHTTP;
		}
		if(wifi_station_get_connect_status() != STATION_GOT_IP)
		{
			debugf("lost wifi connection\n\r");
			_statMainProcess = _MP_init;
		}
		break;
	case _MP_requetteHTTP:
		debugf("_MP_requetteHTTP\n\r");
		if(_cntMainProcess == 1)
		{
			_httpReqRes = 0;
			_sendHttpRequest();
		}
		else
		{
			if(_httpReqRes == 1)// start request fail
			{
				_cntMainProcess = 0;
			}
			if(_httpReqRes == 2)// start request sucess
			{
				_cntMainProcess = 0;
				_statMainProcess = _MP_waitMqttConnect;
			}
		}
		if(_cntMainProcess >= 30)
		{
			debugf("time out requette http\n\r");
			_statMainProcess = _MP_init;
		}
		if(wifi_station_get_connect_status() != STATION_GOT_IP)
		{
			debugf("lost wifi connection\n\r");
			_statMainProcess = _MP_init;
		}
		break;
	case _MP_waitMqttConnect:
		if(_cntMainProcess == 1)
			debugf("_MP_waitMqttConnect\n\r");

		if(_cntMainProcess >= 10)
		{
			debugf("timeOut mqtt connection\n\r");
			_statMainProcess = _MP_init;
		}

		if(isMqttEspReady() == true)
		{
			debugf("mqtt connection\n\r");
			_cntMainProcess = 0;
			_statMainProcess = _MP_verifSendFileData;
		}

		if(wifi_station_get_connect_status() != STATION_GOT_IP)
		{
			debugf("lost wifi connection\n\r");
			_statMainProcess = _MP_init;
		}
		break;
	case _MP_verifSendFileData:
		// if(_cntMainProcess == 1)
		// {
		// 	debugf("_MP_verifSendFileData\n\r");
		// 	// _sdata._init();
		// 	// _sdata._start();
		// }

		// debugf("sD%d ",_sdata._statSendData);

		// if(_sdata._statSendData == _SD_complete
		// || _sdata._statSendData == _SD_nothingToSend)
		// {
		// 	_sdata._init();

		// 	_cntMainProcess = 0;
		// 	_statMainProcess = _MP_allConnect;
		// }
		
		_cntMainProcess = 0;
		_statMainProcess = _MP_allConnect;
		
		break;
	case _MP_allConnect:
		if(_cntMainProcess == 1)
		{
			debugf("_MP_allConnect\n\r");
		
		}

		if(wifi_station_get_connect_status() != STATION_GOT_IP)
		{
			debugf("lost wifi connection\n\r");
			_statMainProcess = _MP_init;
		}
		else
		if(isMqttEspReady() == false)
		{
			debugf("lost mqtt connection\n\r");
			_statMainProcess = _MP_init;
		}
		break;
	}

	if(_smartWifiInProcess >= 0)
	{
		_smartWifiInProcess++;
		if(_smartWifiInProcess >= 30)
		{
			_smartWifiInProcess = _STOP;

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

			_cntMainProcess = 0;
			_statMainProcess = _MP_init;
		}
	}

	// _radiator._processRadiator();

	// _radiator._temperatureMonitoring();

	if((_countSecond % 60) == 0)
		Logger.info("_countSecond : %d",_countSecond);

	_countSecond++;
}

void irom cMainP::_stop()
{
	ETS_GPIO_INTR_DISABLE();

	_cntMainProcess = _STOP;

	_wdgHw._stopWdgCounter(ID_MAINPROCESS_TIMER);
	mainProcess_timer.stop();

	stopMqttEsp();

	ds1820.stop();

	_adc._stop();
}

void irom cMainP::_start()
{
	_cntMainProcess = 0;
	_statMainProcess = _MP_init;

	//_sdata._init();

	ds1820.start();

	_adc._start();
	
	debugf("_START_\n\r");

	mainProcess_timer.initializeMs(DELAY_MAINPROCESS_TIMER, TimerDelegate(&cMainP::_tick_mainP, this)).start();
	_wdgHw._startWdgCounter(ID_MAINPROCESS_TIMER);

    ETS_GPIO_INTR_ENABLE();
}

cMainP mainP(0);

