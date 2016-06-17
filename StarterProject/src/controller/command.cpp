#include <SmingCore/SmingCore.h>

#include "define.h"
#include "version.h"

#include "logger.h"
#include "hw_relay.h"

#include "ota.h"
#include "smartwifi.h"

#include "mainProcess.h"


void ShowInfo()
{
	Logger.info("showinfo");
	Logger.info("DEVICE ID: v%s", Logger.getName().c_str());
	Logger.warm("SDK: v%s", system_get_sdk_version());
	Logger.fatal("Free Heap: %d", system_get_free_heap_size());
	Logger.info("CPU Frequency: %d MHz", system_get_cpu_freq());
	Logger.error("System Chip ID: %x", system_get_chip_id());
	Logger.trace("SPI Flash ID: %x", spi_flash_get_id());
	Logger.info("SPI Flash Size: %d",(1 << ((spi_flash_get_id() >> 16) & 0xff)));
}

void ParseCommand(char* str)
{
	if (!strcmp(str, "connect"))
	{
		// connect to wifi
		WifiStation.config(WIFI_SSID, WIFI_PWD);
		WifiStation.enable(true);
	}
	else
	if (!strcmp(str, "ip"))
	{
		Logger.info("ip= %s mac= %s", WifiStation.getIP().toString().c_str(),WifiStation.getMAC().c_str());
	}
	else
	if (!strcmp(str, "ota"))
	{
		OtaUpdate(OtaDelegate(&cMainP::_stop,&mainP),OtaDelegate(&cMainP::_start,&mainP));
	}
	else
	if (!strcmp(str, "switch"))
	{
		Switch();
	}
	else
	if (!strcmp(str, "smart"))
	{
		wifi_set_opmode(STATION_MODE);
    	Logger.info("startSmartWifi");
		startSmartWifi();
	}
	else
	if (!strcmp(str, "restart"))
	{
		System.restart();
	}
	else
	if (!strcmp(str, "ls"))
	{
		//_fileSys._listFile();
	}
	else
	if (!strcmp(str, "info"))
	{
		ShowInfo();
	}
	else
	if (!strcmp(str, "format"))
	{
		// _fileSys._spiffs_unmount();
		// _fileSys._mountFileSystem(true);
		// System.restart();
	}
	else
	if (!strcmp(str, "relay_on"))
	{
		_relay._On();
		Logger.info("_relay._On");
	}
	else
	if (!strcmp(str, "relay_off"))
	{
		_relay._Off();
		Logger.info("_relay._Off");
	}
	else
	if (!strcmp(str, "calendar_dis"))
	{
		Logger.info("calendar_dis");
		// _radiator._calandarDisplay();
	}
	else
	if (!strcmp(str, "calandar_def"))
	{
		// _radiator._calandarDefault();
		Logger.info("calandar_def");
	}
	else
	if (!strcmp(str, "calandar_sav"))
	{
		// _radiator._encodeCalandar();
		Logger.info("calandar_sav");
	}
	else
	if (!strcmp(str, "calandar_del"))
	{
		// _fileSys._deleteFile("calandar");
		Logger.info("calandar_del");
	}
	else
	if (!strcmp(str, "mode_auto"))
	{
		// _radiator._modeRadiator = _MR_auto;
		Logger.info("mode_auto");
	}
	else
	if (!strcmp(str, "mode_manual"))
	{
		// _radiator._modeRadiator = _MR_manual;
		Logger.info("mode_manual");
	}
	else
	if (!strcmp(str, "help"))
	{
		Logger.info("");
		Logger.info(VERSION);
		Logger.info("available commands:");
		Logger.info("  help - display this message");
		Logger.info("  ip - show current ip address");
		Logger.info("  connect - connect to wifi");
		Logger.info("  smart - lunch smart config");
		Logger.info("  restart - restart the esp8266");
		Logger.info("  switch - switch to the other rom and reboot");
		Logger.info("  ota - perform ota update, switch rom and reboot");
		Logger.info("  info - show esp8266 info");
		Logger.info("  ls - list files in spiffs");
		Logger.info("  format - format file system");
		Logger.info("  relay_on - switch on the relay");
		Logger.info("  relay_off - switch off the relay");
		Logger.info("  mode_auto - use the calendar for the radiator");
		Logger.info("  mode_manual - not use the calendar for the radiator");
		Logger.info("  calendar_dis - display calendar parameter");

		//Logger.info("");
	}
	else
	{
		Logger.info("unknown command");
	}

}

