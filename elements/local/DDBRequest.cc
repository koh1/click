#include <click/config.h>
#include "DDBRequest.hh"
#include "ddbprotocol.hh"

CLICK_DECLS

DDBRequest::DDBRequest() { };
DDBRequest::~DDBRequest() { };

Packet *DDBRequest::gen_ddb_request(String s) {
	struct DDBProto ddb_packet;
	ddb_packet.T = DDBPROTO_REQUEST;
	memcpy(ddb_packet.Data, s.c_str(), DDBPROTO_DATA_LEN);
	ddb_packet.Len = DDBPROTO_LEN_A;
	return Packet::make(headroom, &ddb_packet, sizeof(DDBProto), 0);
}

Packet *DDBRequest::simple_action(Packet *p) {
	Packet *q = NULL;
	int len = strnlen((const char *)p->data(), p->length());
	if (len > DDBPROTO_DATA_LEN)
		len = DDBPROTO_DATA_LEN;
	String s = String(p->data(), len);
	int delta = DDBPROTO_DATA_LEN - len;

	if (delta > 0)
		s.append_fill('\0', delta);

	click_chatter("DEBUG: p->data() = %s\tp->length() = %d", s.c_str(), p->length());

	if (p->length() > 0 && p->length() <= DDBPROTO_DATA_LEN + 1)
		q = gen_ddb_request(s);
	else
		click_chatter("ERROR: Input packet is too big or 0-sized!");

	p->kill();

	return q;
};

CLICK_ENDDECLS
EXPORT_ELEMENT(DDBRequest)
