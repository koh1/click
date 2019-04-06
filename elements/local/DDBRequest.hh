#ifndef CLICK_DDBREQUEST__HH
#define CLICK_DDBREQUEST__HH

#include <click/element.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>

CLICK_DECLS

/*
=c

DDBRequest(TODO)

=s

TODO: Summary

=d

TODO: Complete description

*/
class DDBRequest : public Element {

	Packet *gen_ddb_request(String s);
	int headroom = sizeof(click_ip) + sizeof(click_udp) + sizeof(click_ether);

	public:
		DDBRequest();
		~DDBRequest();

		const char *class_name() const { return "DDBRequest"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return AGNOSTIC; }

		Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS

#endif
