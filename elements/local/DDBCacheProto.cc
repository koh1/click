#include <click/config.h>
// #include <click/TODO.hh>
#include "DDBCacheProto.hh"
#include "ddbprotocol.hh"

CLICK_DECLS

DDBCacheProto::DDBCacheProto() { };
DDBCacheProto::~DDBCacheProto() { };

Packet *DDBCacheProto::pull(int port) {
	Packet *p = input(port).pull();
	return p;
};

void DDBCacheProto::push(int port, Packet *p) {
    output(port).push(p);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DDBCacheProto)
