#include "../../src/SCNSL_broker.hh"
#include "SCNSL_pub.hh"
#include "SCNSL_sub.hh"

#include <exception>
#include <scnsl.hh>
#include <sstream>
#include <systemc>
#include <tlm.h>

using namespace Scnsl::Setup;
using namespace Scnsl::BuiltinPlugin;
using namespace Scnsl::Protocols::Network_Lv4;
using Scnsl::Tracing::Traceable_base_t;

int sc_main(int argc, char *argv[])
{
	// Singleton.
	Scnsl::Setup::Scnsl_t *scnsl = Scnsl::Setup::Scnsl_t::get_instance();

	// Nodes creation:
	Scnsl::Core::Node_t *publisher_node = scnsl->createNode();
	Scnsl::Core::Node_t *broker_node = scnsl->createNode();
	Scnsl::Core::Node_t *subscriber_node = scnsl->createNode();

	float delayVal_pub_broker = 90;
	// float delayVal_broker_sub = delayVal_broker_sub = 90;

	// delay e bitrate, se sono simili va bene uno, altrimenti due
	sc_core::sc_time DELAY(delayVal_pub_broker, sc_core::SC_US);
	unsigned int bitrate = 94200000;
	// unsigned int bitrate_broker_sub = delayVal_broker_sub = 90;

	std::cerr << "DELAY :" << DELAY << std::endl;

	// dobbiamo creare due canali
	CoreChannelSetup_t ccs_pub_broker;
	ccs_pub_broker.channel_type = CoreChannelSetup_t::FULL_DUPLEX;
	ccs_pub_broker.capacity = 100000000;
	ccs_pub_broker.capacity2 = 100000000;
	ccs_pub_broker.delay = DELAY;
	ccs_pub_broker.extensionId = "core";
	ccs_pub_broker.name = "channel_pub_broker";
	Scnsl::Core::Channel_if_t *ch_pub_broker = scnsl->createChannel(ccs_pub_broker);

	CoreChannelSetup_t ccs_broker_sub;
	ccs_broker_sub.channel_type = CoreChannelSetup_t::FULL_DUPLEX;
	ccs_broker_sub.capacity = 100000000;
	ccs_broker_sub.capacity2 = 100000000;
	ccs_broker_sub.delay = DELAY;
	ccs_broker_sub.extensionId = "core";
	ccs_broker_sub.name = "channel_fullduplex";
	Scnsl::Core::Channel_if_t *ch_broker_sub = scnsl->createChannel(ccs_broker_sub);

	const Scnsl::Core::task_id_t publisher_id = 0;
	const Scnsl::Core::task_id_t broker_id = 1;
	const Scnsl::Core::task_id_t subscriber_id = 2;

	SCNSL_pub publisher("Publisher", publisher_id, publisher_node, 1);
	SCNSL_pub broker("Broker", broker_id, broker_node, 1);
	SCNSL_pub subscriber("Subscriber", subscriber_id, subscriber_node, 1);

	// noi dobbiamo creare 3 TCP, uno per ogni macchina

	// Creating the protocol Tcp:
	auto tcp_publisher = new Scnsl::Protocols::Network_Lv4::Lv4Communicator_t("Tcp_publisher", true);
	tcp_publisher->setExtraHeaderSize(14 + 20);
	tcp_publisher->setSegmentSize(
		MAX_ETH_SEGMENT - TCP_BASIC_HEADER_LENGTH - IP_HEADER_MIN_SIZE);
	tcp_publisher->setRto(sc_core::sc_time(200, sc_core::SC_MS));

	auto tcp_broker = new Scnsl::Protocols::Network_Lv4::Lv4Communicator_t("Tcp_broker", true);
	tcp_broker->setExtraHeaderSize(14 + 20);
	tcp_broker->setSegmentSize(
		MAX_ETH_SEGMENT - TCP_BASIC_HEADER_LENGTH - IP_HEADER_MIN_SIZE);
	tcp_broker->setRto(sc_core::sc_time(200, sc_core::SC_MS));

	auto tcp_subscriber = new Scnsl::Protocols::Network_Lv4::Lv4Communicator_t("Tcp_subscriber", true);
	tcp_subscriber->setExtraHeaderSize(14 + 20);
	tcp_subscriber->setSegmentSize(
		MAX_ETH_SEGMENT - TCP_BASIC_HEADER_LENGTH - IP_HEADER_MIN_SIZE);
	tcp_subscriber->setRto(sc_core::sc_time(200, sc_core::SC_MS));

	// 4 bind: p -> b, b -> p, s -> b, b -> s
	// x y z basta che non siano sovrapposti
	// trasmission power e thresold lasciale cosi
	// Binding:
	BindSetup_base_t bsb_publisher_broker;
	bsb_publisher_broker.extensionId = "core";
	bsb_publisher_broker.destinationNode = broker_node;
	bsb_publisher_broker.node_binding.x = 0;
	bsb_publisher_broker.node_binding.y = 0;
	bsb_publisher_broker.node_binding.z = 0;
	bsb_publisher_broker.node_binding.bitrate = bitrate;
	bsb_publisher_broker.node_binding.transmission_power = 1000;
	bsb_publisher_broker.node_binding.receiving_threshold = 1;

	bsb_publisher_broker.socket_binding.socket_active = true;
	bsb_publisher_broker.socket_binding.source_ip = SocketMap::getIP("192.168.0.200");
	bsb_publisher_broker.socket_binding.source_port = 2020;
	bsb_publisher_broker.socket_binding.dest_ip = SocketMap::getIP("192.168.0.201");
	bsb_publisher_broker.socket_binding.dest_port = 2021;

	// bind canale e setup = collegare il cavo ethernet
	scnsl->bind(publisher_node, ch_pub_broker, bsb_publisher_broker);
	// bind sul task
	scnsl->bind(&publisher, &broker, ch_pub_broker, bsb_publisher_broker, tcp_publisher);

	BindSetup_base_t bsb_broker_publisher;
	bsb_broker_publisher.extensionId = "core";
	bsb_broker_publisher.destinationNode = publisher_node;
	bsb_broker_publisher.node_binding.x = 1;
	bsb_broker_publisher.node_binding.y = 1;
	bsb_broker_publisher.node_binding.z = 1;
	bsb_broker_publisher.node_binding.bitrate = bitrate;
	bsb_broker_publisher.node_binding.transmission_power = 1000;
	bsb_broker_publisher.node_binding.receiving_threshold = 1;

	bsb_broker_publisher.socket_binding.socket_active = true;
	bsb_broker_publisher.socket_binding.source_ip = SocketMap::getIP("192.168.0.201");
	bsb_broker_publisher.socket_binding.source_port = 2021;
	bsb_broker_publisher.socket_binding.dest_ip = SocketMap::getIP("192.168.0.200");
	bsb_broker_publisher.socket_binding.dest_port = 2020;

	// bind canale e setup = collegare il cavo ethernet
	scnsl->bind(broker_node, ch_pub_broker, bsb_broker_publisher);
	// bind sul task
	scnsl->bind(&broker, &publisher, ch_pub_broker, bsb_broker_publisher, tcp_broker);

	BindSetup_base_t bsb_broker_subscriber;
	bsb_broker_subscriber.extensionId = "core";
	bsb_broker_subscriber.destinationNode = subscriber_node;
	bsb_broker_subscriber.node_binding.x = 2;
	bsb_broker_subscriber.node_binding.y = 2;
	bsb_broker_subscriber.node_binding.z = 2;
	bsb_broker_subscriber.node_binding.bitrate = bitrate;
	bsb_broker_subscriber.node_binding.transmission_power = 1000;
	bsb_broker_subscriber.node_binding.receiving_threshold = 1;

	bsb_broker_subscriber.socket_binding.socket_active = true;
	bsb_broker_subscriber.socket_binding.source_ip = SocketMap::getIP("192.168.0.201");
	bsb_broker_subscriber.socket_binding.source_port = 2021;
	bsb_broker_subscriber.socket_binding.dest_ip = SocketMap::getIP("192.168.0.202");
	bsb_broker_subscriber.socket_binding.dest_port = 2022;

	// bind canale e setup = collegare il cavo ethernet
	scnsl->bind(broker_node, ch_broker_sub, bsb_broker_subscriber);
	// bind sul task
	scnsl->bind(&broker, &subscriber, ch_broker_sub, bsb_broker_subscriber, tcp_broker);

	BindSetup_base_t bsb_subscriber_broker;
	bsb_subscriber_broker.extensionId = "core";
	bsb_subscriber_broker.destinationNode = subscriber_node;
	bsb_subscriber_broker.node_binding.x = 3;
	bsb_subscriber_broker.node_binding.y = 3;
	bsb_subscriber_broker.node_binding.z = 3;
	bsb_subscriber_broker.node_binding.bitrate = bitrate;
	bsb_subscriber_broker.node_binding.transmission_power = 1000;
	bsb_subscriber_broker.node_binding.receiving_threshold = 1;

	bsb_subscriber_broker.socket_binding.socket_active = true;
	bsb_subscriber_broker.socket_binding.source_ip = SocketMap::getIP("192.168.0.202");
	bsb_subscriber_broker.socket_binding.source_port = 2022;
	bsb_subscriber_broker.socket_binding.dest_ip = SocketMap::getIP("192.168.0.201");
	bsb_subscriber_broker.socket_binding.dest_port = 2021;

	// bind canale e setup = collegare il cavo ethernet
	scnsl->bind(subscriber_node, ch_broker_sub, bsb_subscriber_broker);
	// bind sul task
	scnsl->bind(&subscriber, &broker, ch_broker_sub, bsb_subscriber_broker, tcp_subscriber);

	sc_core::sc_start(sc_core::sc_time(5000, sc_core::SC_SEC));
	sc_core::sc_stop();

	return 0;
}
