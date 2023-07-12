include ../config.mk

.PHONY: all install uninstall clean reallyclean

all : mosquitto

OBJS=	mosquitto.o \
		alias_mosq.o \
		bridge.o \
		bridge_topic.o \
		conf.o \
		conf_includedir.o \
		context.o \
		control.o \
		database.o \
		handle_auth.o \
		handle_connack.o \
		handle_connect.o \
		handle_disconnect.o \
		handle_ping.o \
		handle_pubackcomp.o \
		handle_publish.o \
		handle_pubrec.o \
		handle_pubrel.o \
		handle_suback.o \
		handle_subscribe.o \
		handle_unsuback.o \
		handle_unsubscribe.o \
		keepalive.o \
		logging.o \
		loop.o \
		memory_mosq.o \
		memory_public.o \
		misc_mosq.o \
		mux.o \
		mux_epoll.o \
		mux_poll.o \
		net.o \
		net_mosq.o \
		net_mosq_ocsp.o \
		packet_datatypes.o \
		packet_mosq.o \
		password_mosq.o \
		property_broker.o \
		property_mosq.o \
		persist_read.o \
		persist_read_v234.o \
		persist_read_v5.o \
		persist_write.o \
		persist_write_v5.o \
		plugin.o \
		plugin_public.o \
		read_handle.o \
		retain.o \
		security.o \
		security_default.o \
		send_auth.o \
		send_connack.o \
		send_connect.o \
		send_disconnect.o \
		send_mosq.o \
		send_publish.o \
		send_suback.o \
		send_subscribe.o \
		send_unsuback.o \
		send_unsubscribe.o \
		service.o \
		session_expiry.o \
		signals.o \
		strings_mosq.o \
		subs.o \
		sys_tree.o \
		time_mosq.o \
		topic_tok.o \
		tls_mosq.o \
		utf8_mosq.o \
		util_mosq.o \
		util_topic.o \
		websockets.o \
		will_delay.o \
		will_mosq.o \
		xtreport.o \
		SCNSL_broker.o

mosquitto : ${OBJS}
	${CROSS_COMPILE}${CXX} ${BROKER_LDFLAGS} $^ -o $@ -lscnsl -lsystemc $(BROKER_LDADD)

SCNSL_broker.o : SCNSL_broker.cpp SCNSL_broker.hh mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

mosquitto.o : mosquitto.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

alias_mosq.o : ../lib-broker/alias_mosq.c ../lib-broker/alias_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

bridge.o : bridge.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

bridge_topic.o : bridge_topic.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

conf.o : conf.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

conf_includedir.o : conf_includedir.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

context.o : context.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

control.o : control.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

database.o : database.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

handle_auth.o : handle_auth.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

handle_connack.o : handle_connack.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

handle_connect.o : handle_connect.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

handle_disconnect.o : handle_disconnect.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

handle_ping.o : ../lib-broker/handle_ping.c ../lib-broker/read_handle.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

handle_pubackcomp.o : ../lib-broker/handle_pubackcomp.c ../lib-broker/read_handle.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

handle_publish.o : handle_publish.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

handle_pubrec.o : ../lib-broker/handle_pubrec.c ../lib-broker/read_handle.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

handle_pubrel.o : ../lib-broker/handle_pubrel.c ../lib-broker/read_handle.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

handle_suback.o : ../lib-broker/handle_suback.c ../lib-broker/read_handle.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

handle_subscribe.o : handle_subscribe.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

handle_unsuback.o : ../lib-broker/handle_unsuback.c ../lib-broker/read_handle.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

handle_unsubscribe.o : handle_unsubscribe.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

keepalive.o : keepalive.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

logging.o : logging.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

loop.o : loop.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

memory_mosq.o : ../lib-broker/memory_mosq.c ../lib-broker/memory_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

memory_public.o : memory_public.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

misc_mosq.o : ../lib-broker/misc_mosq.c ../lib-broker/misc_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

mux.o : mux.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

mux_epoll.o : mux_epoll.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

mux_poll.o : mux_poll.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

net.o : net.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

net_mosq_ocsp.o : ../lib-broker/net_mosq_ocsp.c ../lib-broker/net_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

net_mosq.o : ../lib-broker/net_mosq.c ../lib-broker/net_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

password_mosq.o : password_mosq.c password_mosq.h mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

persist_read.o : persist_read.c persist.h mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

persist_read_v234.o : persist_read_v234.c persist.h mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

persist_read_v5.o : persist_read_v5.c persist.h mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

persist_write.o : persist_write.c persist.h mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

persist_write_v5.o : persist_write_v5.c persist.h mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

packet_datatypes.o : ../lib-broker/packet_datatypes.c ../lib-broker/packet_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

packet_mosq.o : ../lib-broker/packet_mosq.c ../lib-broker/packet_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

property_broker.o : property_broker.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

property_mosq.o : ../lib-broker/property_mosq.c ../lib-broker/property_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

plugin.o : plugin.c ../include/mosquitto_plugin.h mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

plugin_public.o : plugin_public.c ../include/mosquitto_plugin.h mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

read_handle.o : read_handle.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

retain.o : retain.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

security.o : security.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

security_default.o : security_default.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

send_auth.o : send_auth.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

send_connect.o : ../lib-broker/send_connect.c ../lib-broker/send_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

send_disconnect.o : ../lib-broker/send_disconnect.c ../lib-broker/send_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

send_connack.o : send_connack.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

send_mosq.o : ../lib-broker/send_mosq.c ../lib-broker/send_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

send_publish.o : ../lib-broker/send_publish.c ../lib-broker/send_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

send_suback.o : send_suback.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

send_subscribe.o : ../lib-broker/send_subscribe.c ../lib-broker/send_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

send_unsuback.o : send_unsuback.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

send_unsubscribe.o : ../lib-broker/send_unsubscribe.c ../lib-broker/send_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

service.o : service.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

session_expiry.o : session_expiry.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

signals.o : signals.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

strings_mosq.o : ../lib-broker/strings_mosq.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

subs.o : subs.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

sys_tree.o : sys_tree.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

time_mosq.o : ../lib-broker/time_mosq.c ../lib-broker/time_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

tls_mosq.o : ../lib-broker/tls_mosq.c
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

topic_tok.o : topic_tok.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

util_mosq.o : ../lib-broker/util_mosq.c ../lib-broker/util_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

util_topic.o : ../lib-broker/util_topic.c ../lib-broker/util_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

utf8_mosq.o : ../lib-broker/utf8_mosq.c
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

websockets.o : websockets.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

will_delay.o : will_delay.c mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

will_mosq.o : ../lib-broker/will_mosq.c ../lib-broker/will_mosq.h
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

xtreport.o : xtreport.c
	${CROSS_COMPILE}${CXX} $(BROKER_CPPFLAGS) $(BROKER_CFLAGS) -c $< -o $@

plugin_defer.so : plugin_defer.c ../include/mosquitto_plugin.h ../include/mosquitto_broker.h mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} -I. -I../lib -fPIC -shared $< -o $@

plugin_debug.so : plugin_debug.c ../include/mosquitto_plugin.../include/h mosquitto_broker.h mosquitto_broker_internal.h
	${CROSS_COMPILE}${CXX} -I. -I../lib -fPIC -shared $< -o $@

install : all
	$(INSTALL) -d "${DESTDIR}$(prefix)/sbin"
	$(INSTALL) ${STRIP_OPTS} mosquitto "${DESTDIR}${prefix}/sbin/mosquitto"
	$(INSTALL) -d "${DESTDIR}$(prefix)/include"
	$(INSTALL) ../include/mosquitto_broker.h "${DESTDIR}${prefix}/include/mosquitto_broker.h"
	$(INSTALL) ../include/mosquitto_plugin.h "${DESTDIR}${prefix}/include/mosquitto_plugin.h"

uninstall :
	-rm -f "${DESTDIR}${prefix}/sbin/mosquitto"
	-rm -f "${DESTDIR}${prefix}/include/mosquitto_broker.h"
	-rm -f "${DESTDIR}${prefix}/include/mosquitto_plugin.h"

clean :
	-rm -f *.o mosquitto *.gcda *.gcno

reallyclean : clean
	-rm -rf *.orig *.db
