#ifndef CLICK_DDBANSWER2__HH
#define CLICK_DDBANSWER2__HH

#include <click/element.hh>
#include <click/hashtable.hh>
#include <clicknet/ether.h>
#include <clicknet/udp.h>

CLICK_DECLS

/*
=c

DDBAnswer(TODO)

=s

TODO: Summary

=d

TODO: Complete description

*/
class DDBAnswer2 : public Element {
	HashTable<String, String> _msgs;
	static int write_callback(const String &s, Element *e, void *vparam, ErrorHandler *errh);
	static String read_callback(Element *e, void *vparam);
	int headroom = sizeof(click_ip) + sizeof(click_udp) + sizeof(click_ether);

	public:
		DDBAnswer2();
		~DDBAnswer2();

		const char *class_name() const { return "DDBAnswer2"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return AGNOSTIC; }

		Packet *simple_action(Packet *p);
		void add_handlers();
};

CLICK_ENDDECLS

#endif
