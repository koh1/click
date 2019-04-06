#ifndef CLICK_DDBCLASSIFIER__HH
#define CLICK_DDBCLASSIFIER__HH

#include <click/element.hh>

CLICK_DECLS

/*
=c

DDBClassifier(TODO)

=s

TODO: Summary

=d

TODO: Complete description

*/
class DDBClassifier : public Element {

	public:
		DDBClassifier();
		~DDBClassifier();

		const char *class_name() const { return "DDBClassifier"; }
		const char *port_count() const { return "1/5"; }
		const char *processing() const { return PUSH; }

		void push(int, Packet *p);

};

CLICK_ENDDECLS

#endif
