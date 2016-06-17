#ifndef _MQTTSERVER_H_
#define __MQTTSERVER_H_

#include "type.h"

#include "MqttClient.h"

void startMqttEsp(char* clientName);
void stopMqttEsp();
bool isMqttEspReady();

extern MqttClient2 mqttEspClient;

extern bool _allConnected();

#endif