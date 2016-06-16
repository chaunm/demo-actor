/*
 ============================================================================
 Name        : DemoActor.c
 Author      : ChauNM
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "jansson.h"
#include <mosquitto.h>
#include "actor.h"
#include "common/ActorParser.h"

PACTOR pPingActor;
PACTOR pPongActor;

void PingActorResponseCallback(void* pParam)
{
	static int nResponseCount;
	char* pingSendTopic;
	char* pingSendMessage;

	char* message = (char*)pParam;
	char** pingSplitMessage = ActorSplitMessage(message);
	if (pingSplitMessage == NULL) return;
	json_t* pingSendJson = json_object();
	json_t* messageJson = json_string("Holla");
	json_object_set(pingSendJson, "message", messageJson);
	json_decref(messageJson);
	pingSendMessage = json_dumps(pingSendJson, JSON_INDENT(4));
	printf("Receive response from Pong\n");
	pingSendTopic = ActorMakeTopicName(pPongActor->guid, "/:request/bonjour");
	ActorSend(pPingActor, pingSendTopic, pingSendMessage, PingActorResponseCallback, TRUE);
	free(pingSendMessage);
	free(pingSendTopic);
	json_decref(pingSendJson);
	nResponseCount++;
	ActorFreeSplitMessage(pingSplitMessage);
	printf("Response count %d\n", nResponseCount);
}

void PongActorRequestCallback(void* pParam)
{
	char* message = (char*)pParam;
	char* sendTopic;
	char* sendMessage;
	const char* messageHeader;
	const char* messageContent;
	char** messageSplit;

	if (message == NULL) return;

	messageSplit = ActorSplitMessage(message);
	if (messageSplit == NULL) return;
	messageHeader = (messageSplit[0]);
	messageContent = (messageSplit[1]);

	json_t* jsonContent = json_loads(messageContent, JSON_DECODE_ANY, NULL);
	json_t* jsonHeader = json_loads(messageHeader, JSON_DECODE_ANY, NULL);
	json_t* jsonFrom = json_object_get(jsonHeader, "from");
	json_t* responseJson;
	// assume that json parse success
	if (jsonFrom == NULL){
		//ActorFreeSplitMessage(messageSplit);
		return;
	}
	printf("receive request from %s\n", json_string_value(jsonFrom));
	sendTopic = ActorMakeTopicName(json_string_value(jsonFrom), "/:response");
	json_decref(jsonFrom);
	printf("Send response to topic %s \n", sendTopic);
	responseJson = json_object();
	json_object_set(responseJson, "request", jsonContent);
	json_t* messageJson = json_string("Holla");
	json_object_set(responseJson, "message", messageJson);
	json_decref(messageJson);
	sendMessage = json_dumps(responseJson, JSON_INDENT(4));
	ActorSend(pPongActor, sendTopic, sendMessage, NULL, TRUE);
	free(sendTopic);
	free(sendMessage);
	json_decref(jsonHeader);
	json_decref(jsonContent);
	json_decref(responseJson);
	ActorFreeSplitMessage(messageSplit);
	return;
}

int main()
{
	json_t* pingSendJson = json_object();
	char* pingSendMessage;
	char* pingSendTopic;
	BOOL FirstSend = FALSE;
	/* Mosquitto library init */
	mosquitto_lib_init();
	/* Create actor to start communication */
	pPongActor = ActorCreate("system/pong", "ping");
	pPingActor = ActorCreate("system/ping", "ping");
	if ((pPingActor == NULL) || (pPongActor == NULL)) return 1;
	/* make message for the first time  */
	json_t* messageJson = json_string("Holla");
	json_object_set(pingSendJson, "message", messageJson);
	json_decref(messageJson);
	pingSendMessage = json_dumps(pingSendJson, JSON_INDENT(4));
	json_decref(pingSendJson);
	ActorRegisterCallback(pPongActor, ":request/bonjour", PongActorRequestCallback, CALLBACK_RETAIN);
	pingSendTopic = ActorMakeTopicName(pPongActor->guid, "/:request/bonjour");
	while(1)
	{
		ActorProcessEvent(pPingActor);
		ActorProcessEvent(pPongActor);
		mosquitto_loop(pPingActor->client, 0, 1);
		mosquitto_loop(pPongActor->client, 0, 1);
		if(FirstSend == FALSE)
		{
			printf("send initialize message\n");
			FirstSend = TRUE;
			ActorSend(pPingActor, pingSendTopic, pingSendMessage, PingActorResponseCallback, TRUE);
			free(pingSendMessage);
			free(pingSendTopic);
		}
		usleep(10000);
	}
	mosquitto_disconnect(pPongActor->client);
	mosquitto_disconnect(pPingActor->client);
	mosquitto_destroy(pPongActor->client);
	mosquitto_destroy(pPingActor->client);
	ActorDelete(pPingActor);
	ActorDelete(pPongActor);
	mosquitto_lib_cleanup();
	return 0;
}

