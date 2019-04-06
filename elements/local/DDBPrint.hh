#ifndef CLICK_DDBPRINT__HH
#define CLICK_DDBPRINT__HH

#include <click/element.hh>

CLICK_DECLS

/*
=c

DDBPrint(TODO)

=s

TODO: Summary

=d

TODO: Complete description

*/
class DDBPrint : public Element {
	//TODO: Add private attributes

	public:
		DDBPrint();
		~DDBPrint();

		const char *class_name() const { return "DDBPrint"; }
		const char *port_count() const { return "1/1"; }
		const char *processing() const { return AGNOSTIC; }

		Packet *simple_action(Packet *p);
};

CLICK_ENDDECLS

#endif
