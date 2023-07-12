// Davide Tonin
// SCNSL Subscriber class definition

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

#include "../include/SCNSL_sub.hh"

#define COUNT 3

using namespace Scnsl::Protocols::Network_Lv4;

SCNSL_sub::SCNSL_sub(const sc_core::sc_module_name modulename,
					 const task_id_t id,
					 Scnsl::Core::Node_t *n,
					 const size_t proxies) : // Parents:
											 NetworkAPI_Task_if_t(modulename, id, n, proxies, DEFAULT_WMEM)
{
	// nothing to do
}

SCNSL_sub::~SCNSL_sub()
{
	// Nothing to do.
}

void SCNSL_sub::main()
{
	// il broker parte subito
	wait(4,sc_core::SC_SEC);
    initTime();

	int rc;
	int i;
	struct mosquitto_message *msg;

	mosquitto_lib_init();

	rc = mosquitto_subscribe_simple(
			&msg, COUNT, true,
			"irc/#", 0,
			"test.mosquitto.org", 1883,
			NULL, 60, true,
			NULL, NULL,
			NULL, NULL);

	if(rc){
		printf("Error: %s\n", mosquitto_strerror(rc));
		mosquitto_lib_cleanup();
		return void();
	}

	for(i=0; i<COUNT; i++){
		printf("%s %s\n", msg[i].topic, (char *)msg[i].payload);
		mosquitto_message_free_contents(&msg[i]);
	}
	free(msg);
}