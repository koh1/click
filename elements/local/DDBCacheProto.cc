#include <click/config.h>
// #include <click/TODO.hh>
#include "DDBCacheProto.hh"
#include "DummyProto.hh"

CLICK_DECLS

DDBCacheProto::DDBCacheProto() { };
DDBCacheProto::~DDBCacheProto() { };

Packet *DDBCacheProto::simple_action(Packet *p) {
	// TODO: fill

	return p;
};

CLICK_ENDDECLS
EXPORT_ELEMENT(DDBCacheProto)
