// Davide Tonin
// SCNS Broker header

#include <systemc>
#include <scnsl.hh>
#include <map>
#include <scnsl/system_calls/TimedSyscalls.hh>
#include <stdlib.h>
#include "../../mosquitto-scnsl-test/include/mosquitto.h"

class SCNSL_broker : public Scnsl::Protocols::Network_Lv4::NetworkAPI_Task_if_t
{
public:
	SCNSL_broker(const sc_core::sc_module_name modulename,
				 const task_id_t id,
				 Scnsl::Core::Node_t *n,
				 const size_t proxies);

	/// @brief Virtual destructor.
	virtual ~SCNSL_broker();

	/* mosquitto shouldn't run as root.
	 * This function will attempt to change to an unprivileged user and group if
	 * running as root. The user is given in config->user.
	 * Returns 1 on failure (unknown user, setuid/setgid failure)
	 * Returns 0 on success.
	 * Note that setting config->user to "root" does not produce an error, but it
	 * strongly discouraged.
	 */
	static int drop_privileges(struct mosquitto__config *config);
	static void mosquitto__daemonise(void);
	static void listener__set_defaults(struct mosquitto__listener *listener);
	static void listeners__reload_all_certificates(void);
	static int listeners__start_single_mqtt(struct mosquitto__listener *listener);
	static int listeners__add_local(const char *host, uint16_t port);
	static int listeners__start_local_only(void);
	static int listeners__start(void);
	static void listeners__stop(void);
	static void signal__setup(void);
	static int pid__write(void);

private:
	static struct mosquitto_db db;
	static struct mosquitto__listener_sock *listensock;
	static int listensock_count;
	static int listensock_index;
	static bool flag_reload;
	static bool flag_db_backup;
	static bool flag_tree_print;
	static int run;

	void main() override;
};