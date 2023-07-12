// Davide Tonin
// SCNS Publisher header

#include <systemc>
#include <scnsl.hh>
#include <map>
#include <scnsl/system_calls/TimedSyscalls.hh>
#include <stdlib.h>
#include "../../mosquitto-scnsl-test/include/mosquitto.h"


class SCNSL_sub : public Scnsl::Protocols::Network_Lv4::NetworkAPI_Task_if_t
{
public:
	SCNSL_sub(const sc_core::sc_module_name modulename,
				 const task_id_t id,
				 Scnsl::Core::Node_t *n,
				 const size_t proxies);

	/// @brief Virtual destructor.
	virtual ~SCNSL_sub();

private:
	void main() override;
};