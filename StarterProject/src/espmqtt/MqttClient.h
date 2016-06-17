/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/anakod/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 ****/

#ifndef _MqttClient2_H_
#define _MqttClient2_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "mqtt.h"

class MqttClient2
{
public:

	 MqttClient2();

	void set(String serverHost, int serverPort,
			MqttCallback mqttConnectedCallback = NULL,
			MqttCallback mqttDisconnectedCallback = NULL,
			MqttCallback mqttPublishedCallback = NULL,
			MqttDataCallback mqttDataCallback = NULL);

	virtual ~MqttClient2();

	bool connect(String clientName);
	bool connect(String idClient, String username, String password,
			int keepAliveTime, bool cleanSession);

	void disconnect();

	void setLwt(String will_topic, String will_msg, int will_qos,
			bool will_retain);

	bool publishWithQoS(char* topic, char* message, int length, int qoS,
			bool retained);
	//bool publishWithQoS(String topic, String message, int qoS, bool retained);


	bool subscribe(String topic, int qos);

private:
	String server;
	int port;

	MQTT_Client mqttClient;

	MqttCallback mqttConnectedCb;
	MqttCallback mqttDisconnectedCb;
	MqttCallback mqttPublishedCb;
	MqttDataCallback mqttDataCb;

};

#endif 
