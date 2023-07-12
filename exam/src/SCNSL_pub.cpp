// Davide Tonin
// SCNSL Publisher class definition

#include <systemc>
#include <scnsl.hh>
#include <map>
#include <scnsl/system_calls/TimedSyscalls.hh>
#include <stdlib.h>
#include "../../mosquitto-scnsl-test/include/mosquitto.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/SCNSL_pub.hh"

using namespace Scnsl::Protocols::Network_Lv4;

SCNSL_pub::SCNSL_pub(const sc_core::sc_module_name modulename,
					 const task_id_t id,
					 Scnsl::Core::Node_t *n,
					 const size_t proxies) : // Parents:
											 NetworkAPI_Task_if_t(modulename, id, n, proxies, DEFAULT_WMEM)
{
	// nothing to do
}

SCNSL_pub::~SCNSL_pub()
{
	// Nothing to do.
}

void SCNSL_pub::on_connect(struct mosquitto *mosq, void *obj, int reason_code)
{
	/* Print out the connection result. mosquitto_connack_string() produces an
	 * appropriate string for MQTT v3.x clients, the equivalent for MQTT v5.0
	 * clients is mosquitto_reason_string().
	 */
	printf("on_connect: %s\n", mosquitto_connack_string(reason_code));
	if (reason_code != 0)
	{
		/* If the connection fails for any reason, we don't want to keep on
		 * retrying in this example, so disconnect. Without this, the client
		 * will attempt to reconnect. */
		mosquitto_disconnect(mosq);
	}

	/* You may wish to set a flag here to indicate to your application that the
	 * client is now connected. */
}

void SCNSL_pub::publish(struct mosquitto *mosq)
{
	char payload[20];
	int temp;
	int rc;

	sleep(1); /* Prevent a storm of messages - this pretend sensor works at 1Hz */

	/* Get our pretend data */
	temp = random() % 100;
	/* Print it to a string for easy human reading - payload format is highly
	 * application dependent. */
	snprintf(payload, sizeof(payload), "%d", temp);

	/* Publish the message
	 * mosq - our client instance
	 * *mid = NULL - we don't want to know what the message id for this message is
	 * topic = "example/temperature" - the topic on which this message will be published
	 * payloadlen = strlen(payload) - the length of our payload in bytes
	 * payload - the actual payload
	 * qos = 2 - publish with QoS 2 for this example
	 * retain = false - do not use the retained message feature for this message
	 */
	rc = mosquitto_publish(mosq, NULL, "exam", strlen(payload), payload, 2, false);
	if (rc != MOSQ_ERR_SUCCESS)
	{
		fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(rc));
	}
}

void SCNSL_pub::main()
{
	// il broker parte subito
	wait(2,sc_core::SC_SEC);
    initTime();

	struct mosquitto *mosq;
	int rc;

	/* Required before calling other mosquitto functions */
	mosquitto_lib_init();

	/* Create a new client instance.
	 * id = NULL -> ask the broker to generate a client id for us
	 * clean session = true -> the broker should remove old sessions when we connect
	 * obj = NULL -> we aren't passing any of our private data for callbacks
	 */
	mosq = mosquitto_new(NULL, true, NULL);
	if (mosq == NULL)
	{
		fprintf(stderr, "Error: Out of memory.\n");
		return void();
	}

	/* Configure callbacks. This should be done before connecting ideally. */
	mosquitto_connect_callback_set(mosq, SCNSL_pub::on_connect);

	/* Connect to test.mosquitto.org on port 1883, with a keepalive of 60 seconds.
	 * This call makes the socket connection only, it does not complete the MQTT
	 * CONNECT/CONNACK flow, you should use mosquitto_loop_start() or
	 * mosquitto_loop_forever() for processing net traffic. */
	rc = mosquitto_connect(mosq, "test.mosquitto.org", 1883, 60);
	if (rc != MOSQ_ERR_SUCCESS)
	{
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
		return void();
	}

	/* Run the network loop in a background thread, this call returns quickly. */
	rc = mosquitto_loop_start(mosq);
	if (rc != MOSQ_ERR_SUCCESS)
	{
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
		return void();
	}

	/* At this point the client is connected to the network socket, but may not
	 * have completed CONNECT/CONNACK.
	 * It is fairly safe to start queuing messages at this point, but if you
	 * want to be really sure you should wait until after a successful call to
	 * the connect callback.
	 * In this case we know it is 1 second before we start publishing.
	 */
	while (1)
	{
		SCNSL_pub::publish(mosq);
	}

	mosquitto_lib_cleanup();
}