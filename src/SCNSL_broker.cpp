// Davide Tonin
// SCNSL Broker class definition

#include "../config.h"

#include <unistd.h>
#include <grp.h>
#include <assert.h>
#include <pwd.h>

#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "mosquitto_broker_internal.h"
#include "../lib-broker/memory_mosq.h"
#include "../lib-broker/misc_mosq.h"
#include "../lib-broker/util_mosq.h"

#include <scnsl/system_calls/NetworkSyscalls.hh>
#include <scnsl/system_calls/TimedSyscalls.hh>
using namespace Scnsl::Syscalls;
#define fd_set Scnsl::Syscalls::fd_set

#include <sstream>
#include <stdlib.h>
#include <string>

#include "SCNSL_broker.hh"

using namespace Scnsl::Protocols::Network_Lv4;

struct mosquitto_db SCNSL_broker::db;
int SCNSL_broker::run;

struct mosquitto__listener_sock SCNSL_broker::*listensock = NULL;
int SCNSL_broker::listensock_count = 0;
int SCNSL_broker::listensock_index = 0;

bool SCNSL_broker::flag_reload = false;
bool SCNSL_broker::flag_db_backup = false;
bool SCNSL_broker::flag_tree_print = false;

SCNSL_broker::SCNSL_broker(const sc_core::sc_module_name modulename,
						   const task_id_t id,
						   Scnsl::Core::Node_t *n,
						   const size_t proxies) : // Parents:
												   NetworkAPI_Task_if_t(modulename, id, n, proxies, DEFAULT_WMEM)
{
	// nothing to do
}

SCNSL_broker::~SCNSL_broker()
{
	// Nothing to do.
}

void SCNSL_broker::listener__set_defaults(struct mosquitto__listener *listener)
{
	listener->security_options.allow_anonymous = -1;
	listener->security_options.allow_zero_length_clientid = true;
	listener->protocol = mp_mqtt;
	listener->max_connections = -1;
	listener->max_qos = 2;
	listener->max_topic_alias = 10;
}

int SCNSL_broker::listeners__start_single_mqtt(struct mosquitto__listener *listener)
{
	int i;
	struct mosquitto__listener_sock *listensock_new;

	if (net__socket_listen(listener))
	{
		return 1;
	}
	SCNSL_broker::listensock_count += listener->sock_count;
	listensock_new = (struct mosquitto__listener_sock *)mosquitto__realloc(listensock, sizeof(struct mosquitto__listener_sock) * (size_t)SCNSL_broker::listensock_count);
	if (!listensock_new)
	{
		return 1;
	}
	listensock = listensock_new;

	for (i = 0; i < listener->sock_count; i++)
	{
		if (listener->socks[i] == INVALID_SOCKET)
		{
			return 1;
		}
		listensock[SCNSL_broker::listensock_index].sock = listener->socks[i];
		listensock[SCNSL_broker::listensock_index].listener = listener;
		SCNSL_broker::listensock_index++;
	}
	return MOSQ_ERR_SUCCESS;
}

int SCNSL_broker::listeners__add_local(const char *host, uint16_t port)
{
	struct mosquitto__listener *listeners;
	listeners = db.config->listeners;

	listener__set_defaults(&listeners[db.config->listener_count]);
	listeners[db.config->listener_count].security_options.allow_anonymous = true;
	listeners[db.config->listener_count].port = port;
	listeners[db.config->listener_count].host = mosquitto__strdup(host);
	if (listeners[db.config->listener_count].host == NULL)
	{
		return MOSQ_ERR_NOMEM;
	}
	if (listeners__start_single_mqtt(&listeners[db.config->listener_count]))
	{
		mosquitto__free(listeners[db.config->listener_count].host);
		listeners[db.config->listener_count].host = NULL;
		return MOSQ_ERR_UNKNOWN;
	}
	db.config->listener_count++;
	return MOSQ_ERR_SUCCESS;
}

int SCNSL_broker::listeners__start_local_only(void)
{
	/* Attempt to open listeners bound to 127.0.0.1 and ::1 only */
	int i;
	int rc;
	struct mosquitto__listener *listeners;

	listeners = (struct mosquitto__listener *)mosquitto__realloc(db.config->listeners, 2 * sizeof(struct mosquitto__listener));
	if (listeners == NULL)
	{
		return MOSQ_ERR_NOMEM;
	}
	memset(listeners, 0, 2 * sizeof(struct mosquitto__listener));
	db.config->listener_count = 0;
	db.config->listeners = listeners;

	log__printf(NULL, MOSQ_LOG_WARNING, "Starting in local only mode. Connections will only be possible from clients running on this machine.");
	log__printf(NULL, MOSQ_LOG_WARNING, "Create a configuration file which defines a listener to allow remote access.");
	log__printf(NULL, MOSQ_LOG_WARNING, "For more details see https://mosquitto.org/documentation/authentication-methods/");
	if (db.config->cmd_port_count == 0)
	{
		rc = listeners__add_local("127.0.0.1", 1883);
		if (rc == MOSQ_ERR_NOMEM)
			return MOSQ_ERR_NOMEM;
		rc = listeners__add_local("::1", 1883);
		if (rc == MOSQ_ERR_NOMEM)
			return MOSQ_ERR_NOMEM;
	}
	else
	{
		for (i = 0; i < db.config->cmd_port_count; i++)
		{
			rc = listeners__add_local("127.0.0.1", db.config->cmd_port[i]);
			if (rc == MOSQ_ERR_NOMEM)
				return MOSQ_ERR_NOMEM;
			rc = listeners__add_local("::1", db.config->cmd_port[i]);
			if (rc == MOSQ_ERR_NOMEM)
				return MOSQ_ERR_NOMEM;
		}
	}

	if (db.config->listener_count > 0)
	{
		return MOSQ_ERR_SUCCESS;
	}
	else
	{
		return MOSQ_ERR_UNKNOWN;
	}
}

int SCNSL_broker::listeners__start(void)
{
	int i;

	SCNSL_broker::listensock_count = 0;

	if (db.config->local_only)
	{
		if (listeners__start_local_only())
		{
			db__close();
			if (db.config->pid_file)
			{
				(void)remove(db.config->pid_file);
			}
			return 1;
		}
		return MOSQ_ERR_SUCCESS;
	}

	for (i = 0; i < db.config->listener_count; i++)
	{
		if (db.config->listeners[i].protocol == mp_mqtt)
		{
			if (listeners__start_single_mqtt(&db.config->listeners[i]))
			{
				db__close();
				if (db.config->pid_file)
				{
					(void)remove(db.config->pid_file);
				}
				return 1;
			}
		}
	}
	if (listensock == NULL)
	{
		log__printf(NULL, MOSQ_LOG_ERR, "Error: Unable to start any listening sockets, exiting.");
		return 1;
	}
	return MOSQ_ERR_SUCCESS;
}

void SCNSL_broker::listeners__stop(void)
{
	int i;

	for (i = 0; i < SCNSL_broker::listensock_count; i++)
	{
		if (listensock[i].sock != INVALID_SOCKET)
		{
			COMPAT_CLOSE(listensock[i].sock);
		}
	}
	mosquitto__free(listensock);
}

void SCNSL_broker::signal__setup(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGTERM, handle_sigint);
#ifdef SIGHUP
	signal(SIGHUP, handle_sighup);
#endif
}

int SCNSL_broker::pid__write(void)
{
	FILE *pid;

	if(db.config->pid_file){
		pid = mosquitto__fopen(db.config->pid_file, "wt", false);
		if(pid){
			fprintf(pid, "%d", getpid());
			fclose(pid);
		}else{
			log__printf(NULL, MOSQ_LOG_ERR, "Error: Unable to write pid file.");
			return 1;
		}
	}
	return MOSQ_ERR_SUCCESS;
}

void SCNSL_broker::main()
{
	// il broker parte subito
	wait(0,sc_core::SC_SEC);
    initTime();

	struct mosquitto__config config;
	int rc;
	struct timeval tv;
	struct mosquitto *ctxt, *ctxt_tmp;

	gettimeofday(&tv, NULL);
	srand((unsigned int)(tv.tv_sec + tv.tv_usec));

	memset(&db, 0, sizeof(struct mosquitto_db));
	db.now_s = mosquitto_time();
	db.now_real_s = Scnsl::Syscalls::time(NULL);

	net__broker_init();

	config__init(&config);
	db.config = &config;

	if(SCNSL_broker::pid__write()) return void();

	rc = db__open(&config);
	if(rc != MOSQ_ERR_SUCCESS){
		log__printf(NULL, MOSQ_LOG_ERR, "Error: Couldn't open database.");
		return void();
	}

	/* Initialise logging only after initialising the database in case we're
	 * logging to topics */
	if(log__init(&config)){
		return void();
	}
	log__printf(NULL, MOSQ_LOG_INFO, "mosquitto version %s starting", 1);
	if(db.config_file){
		log__printf(NULL, MOSQ_LOG_INFO, "Config loaded from %s.", db.config_file);
	}else{
		log__printf(NULL, MOSQ_LOG_INFO, "Using default config.");
	}

	rc = mosquitto_security_module_init();
	if(rc) return void();
	rc = mosquitto_security_init(false);
	if(rc) return void();

	/* After loading persisted clients and ACLs, try to associate them,
	 * so persisted subscriptions can start storing messages */
	HASH_ITER(hh_id, db.contexts_by_id, ctxt, ctxt_tmp){
		if(ctxt && !ctxt->clean_start && ctxt->username){
			rc = acl__find_acls(ctxt);
			if(rc){
				log__printf(NULL, MOSQ_LOG_WARNING, "Failed to associate persisted user %s with ACLs, "
					"likely due to changed ports while using a per_listener_settings configuration.", ctxt->username);
			}
		}
	}

	if(SCNSL_broker::listeners__start()) return void();

	rc = mux__init(listensock, SCNSL_broker::listensock_count);
	if(rc) return void();

	SCNSL_broker::signal__setup();

	log__printf(NULL, MOSQ_LOG_INFO, "mosquitto version %s running", 1);

	SCNSL_broker::run = 1;
	rc = mosquitto_main_loop(listensock, SCNSL_broker::listensock_count);

	log__printf(NULL, MOSQ_LOG_INFO, "mosquitto version %s terminating", 1);

	/* FIXME - this isn't quite right, all wills with will delay zero should be
	 * sent now, but those with positive will delay should be persisted and
	 * restored, pending the client reconnecting in time. */
	HASH_ITER(hh_id, db.contexts_by_id, ctxt, ctxt_tmp){
		context__send_will(ctxt);
	}
	will_delay__send_all();

	session_expiry__remove_all();

	SCNSL_broker::listeners__stop();

	HASH_ITER(hh_id, db.contexts_by_id, ctxt, ctxt_tmp){
		{
			context__cleanup(ctxt, true);
		}
	}
	HASH_ITER(hh_sock, db.contexts_by_sock, ctxt, ctxt_tmp){
		context__cleanup(ctxt, true);
	}

	context__free_disused();

	db__close();

	mosquitto_security_module_cleanup();

	if(config.pid_file){
		(void)remove(config.pid_file);
	}

	log__close(&config);
	config__cleanup(db.config);
	net__broker_cleanup();
}