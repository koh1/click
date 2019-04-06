#ifndef CLICK_DDBLOG__HH
#define CLICK_DDBLOG__HH

#include <click/element.hh>
#include <click/timer.hh>

CLICK_DECLS

/*
=c

DDBLog(TODO)

=s

TODO: Summary

=d

TODO: Complete description

*/
class DDBLog : public Element {
	Vector<String> _answers;
	Timer _timer;
	unsigned int _tick;

	void run_timer(Timer *t);

	public:
		DDBLog();
		~DDBLog();

		const char *class_name() const { return "DDBLog"; }
		const char *port_count() const { return "1/0"; }
		const char *processing() const { return AGNOSTIC; }

		Packet *simple_action(Packet *p);
		int initialize(ErrorHandler *);
		int configure(Vector<String> &conf, ErrorHandler *errh);
};

CLICK_ENDDECLS

#endif
