#ifndef INCLUDE_OTA_H_
#define INCLUDE_OTA_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "type.h"
#include "define.h"

typedef Delegate<void()> OtaDelegate;

/*
 #define ROM_0_URL  "http://192.168.0.22:2003/ota/" DEVICE_ID "/rom0.bin"
 #define ROM_1_URL  "http://192.168.0.22:2003/ota/" DEVICE_ID "/rom1.bin"
 #define SPIFFS_URL "http://192.168.0.22:2003/ota/" DEVICE_ID "/spiff_rom.bin"
 */

#define ROM_0_URL  SERVER_URL "/ota/" DEVICE_ID "/rom0.bin"
#define ROM_1_URL  SERVER_URL "/ota/" DEVICE_ID "/rom1.bin"
#define SPIFFS_URL SERVER_URL "/ota/" DEVICE_ID "/spiff_rom.bin"

void OtaUpdate(OtaDelegate _stopBeforeOta = NULL, OtaDelegate _restartAfterOta = NULL);
void Switch();

#endif /* INCLUDE_OTA_H_ */
