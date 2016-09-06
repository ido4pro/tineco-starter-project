#include <SmingCore/SmingCore.h>

#include "webserver.h"

#include "relayRoute.h"
#include "hw_relay.h"

void onChangeStateRelay(HttpRequest &request, HttpResponse &response) {
	
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();
	json["status"] = (bool) true;
	json["date"]= SystemClock.getSystemTimeString(eTZ_UTC);
	
	//_relay._Switch();

	response.sendJsonObject(stream);
}
