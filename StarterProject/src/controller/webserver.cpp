#include <SmingCore/SmingCore.h>

#include "webserver.h"

#include "relayRoute.h"


HttpServer server;

void onAjaxFrequency(HttpRequest &request, HttpResponse &response) {
	int freq = request.getQueryParameter("value").toInt();

	debugf("Query value= %d\n", freq);

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();
	
	json["cpuFrequency"] = (int) System.getCpuFrequency();
	json["date"]= SystemClock.getSystemTimeString(eTZ_UTC);

	response.sendJsonObject(stream);
}

void onIndex(HttpRequest &request, HttpResponse &response) {

	debugf("Default Web Handler");
	response.sendString("?*?");
}

void startWebServer() {
	server.listen(80);

	server.addPath("/api/frequency", onAjaxFrequency);
	
	server.addPath("/api/changeStateRelay", onChangeStateRelay);
	
	server.setDefaultHandler(onIndex);

	debugf("\r\n== WEB SERVER STARTED ==\r\n");
	//Serial.println(WifiStation.getIP());
	//Serial.println("==============================\r\n");
}
