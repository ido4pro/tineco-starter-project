#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "smartwifi.h"

void smartconfigDone(sc_status status, void *pdata) {
	station_config *sta_conf = reinterpret_cast<station_config *>(pdata);
	char *ssid = reinterpret_cast<char *>(sta_conf->ssid);
	char *password = reinterpret_cast<char *>(sta_conf->password);

	switch (status) {
	case SC_STATUS_WAIT:
		debugf("SC_STATUS_WAIT\n");
		break;
	case SC_STATUS_FIND_CHANNEL:
		debugf("SC_STATUS_FIND_CHANNEL\n");
		break;
	case SC_STATUS_GETTING_SSID_PSWD:
		debugf("SC_STATUS_GETTING_SSID_PSWD\n");
		break;
	case SC_STATUS_LINK:
		debugf("SC_STATUS_LINK\n");
		WifiStation.config(ssid, password);
		break;
	case SC_STATUS_LINK_OVER:
		debugf("SC_STATUS_LINK_OVER\n");
		smartconfig_stop();
		break;
	}
}

void startSmartWifi() {
	smartconfig_set_type (SC_TYPE_ESPTOUCH_AIRKISS);
	smartconfig_start(smartconfigDone);
}

void stopSmartWifi() {
	smartconfig_stop();
}
