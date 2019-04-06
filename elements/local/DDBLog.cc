#include <click/config.h>
#include <click/args.hh>
#include "DDBLog.hh"
#include "ddbprotocol.hh"

CLICK_DECLS

DDBLog::DDBLog() : _timer((Element *) this){ };
DDBLog::~DDBLog() { };

int DDBLog::initialize(ErrorHandler *) {
	_timer.initialize((Element *) this);
	_timer.schedule_now();

	return 0;
}

int DDBLog::configure(Vector<String> &conf, ErrorHandler *errh) {
	if (Args(conf, this, errh).read_or_set("TICK", _tick, 5).complete() < 0)
		return -1;
	return 0;
}

void DDBLog::run_timer(Timer *t) {
	assert(&_timer == t);
	_timer.reschedule_after_sec(_tick);

	if (_answers.empty()) {
		click_chatter("LOG is empty");
		return;
	}

	click_chatter("------ LOG ------");
	int i = 0;

	for (Vector<String>::iterator it = _answers.begin(); it && i < _answers.size(); it++, i++)
		click_chatter("[%d] %s", i, it->c_str());
	click_chatter("------ END LOG ------");
}

Packet *DDBLog::simple_action(Packet *p) {
	struct DDBProto *proto = (struct DDBProto*) p->data();
	String s;

	s += "{ Len: ";
	s += String(proto->Len);
	s += ", Data: '";
	s += String(proto->Data, strnlen(proto->Data, DDBPROTO_DATA_LEN)).printable();
	s += "' }";

	_answers.push_back(s);
	p->kill();

	return NULL;
};

CLICK_ENDDECLS
EXPORT_ELEMENT(DDBLog)
