#include "mqttServer.h"

#include "define.h"
#include "logger.h"

#include "command.h"
#include "MqttClient.h"


char levelTopic[30];
char commandTopic[30];
char configTopic[30];
char calendarTopic[30];

bool mqttEspReady;
MqttClient2 mqttEspClient;

bool irom _allConnected()
{
	if(isMqttEspReady() == true
	&& wifi_station_get_connect_status() == STATION_GOT_IP)
		return true;
	else
		return false;
}


bool isMqttEspReady()
{
	return mqttEspReady;
}


void mqttEspConnectedCb(uint32_t *args)
{

	debugf("MQTT ESP: Connected\r\n");

	mqttEspClient.subscribe(levelTopic, 0);
	mqttEspClient.subscribe(commandTopic, 0);
	mqttEspClient.subscribe(configTopic, 0);
	mqttEspClient.subscribe(calendarTopic, 0);

	mqttEspReady = true;
}

void mqttEspDisconnectedCb(uint32_t *args)
{
	debugf("MQTT ESP: Disconnected\r\n");
	mqttEspReady = false;
}

void mqttEspPublishedCb(uint32_t *args)
{

	MQTT_Client* client = (MQTT_Client*) args;

	debugf("MQTT: Published %d/%d\r\n",client->msgQueue.rb.fill_cnt,client->msgQueue.rb.size);
}

void mqttEspDataCb(uint32_t *args, const char* topic, uint32_t topic_len,const char *data, uint32_t data_len)
{
	char *topicBuf = (char*) os_zalloc(topic_len + 1), *dataBuf = (char*) os_zalloc(data_len + 1);

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

	debugf("\n\rReceive topic:%s, data: %s \r\n", topicBuf, dataBuf);

	int lenTopic = strlen(topicBuf);

	if (lenTopic > 6 && !strcmp(topicBuf + lenTopic - 6, "/level"))
	{
		debugf("receive level\n\r");

		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(dataBuf);

		int newLevel = atoi(root["level"]);
		debugf("level = %d\n\r", newLevel);

		Logger.setLevel((log_type) newLevel);

	}
	else
	if (lenTopic > 8 && !strcmp(topicBuf + lenTopic - 8, "/command"))
	{
		debugf("receive command\n\r");

		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(dataBuf);
		
		const char* command = root["command"];

		ParseCommand((char*) command);

	}
	else
	if (lenTopic > 7 && !strcmp(topicBuf + lenTopic - 7, "/config"))
	{
		debugf("receive config \n\r");

		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(dataBuf);

		if(root["fictif"].success()) // exemple
		{
			int newfictif = root["fictif"];
			debugf("fictif = %d\n\r", newfictif);
		}
		if(root["autre"].success()) // exemple
		{
			int newautre = root["autre"];
			debugf("autre = %d\n\r", newautre);
		}
	}
	else
	if (lenTopic > 7 && !strcmp(topicBuf + lenTopic - 13, "/calendar/set"))
	{
		debugf("receive calandar %d \n\r",data_len);
		debugf("data calandar : %s\n\r",dataBuf);
		
		//TODO

		// if(_radiator._decodeCalandar(dataBuf,data_len) == 0)
		// 	_fileSys._writeCalandarFile(dataBuf,data_len);
	}

	os_free(topicBuf);
	os_free(dataBuf);
}

// Will be called when WiFi station was connected to AP
void startMqttEsp(char* clientName)
{
	sprintf(levelTopic, "%s/level", clientName);
	sprintf(commandTopic, "%s/command", clientName);
	sprintf(configTopic, "%s/config", clientName);
	sprintf(calendarTopic, "%s/calendar/set", clientName);

	debugf("Mqtt esp server : %s\n\r", MQTT_URL);
	debugf("Mqtt esp name : %s\n\r", clientName);

	mqttEspClient.set(MQTT_URL, 1883, mqttEspConnectedCb, mqttEspDisconnectedCb,mqttEspPublishedCb, mqttEspDataCb);

	mqttEspClient.connect(clientName, String(""), String(""), 120, 0);
}

void stopMqttEsp()
{
	mqttEspClient.disconnect();
}