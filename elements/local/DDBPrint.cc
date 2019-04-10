#include <click/config.h>
#include <click/glue.hh>

#include "DDBPrint.hh"
#include "ddbprotocol.hh"

CLICK_DECLS

DDBPrint::DDBPrint() { };
DDBPrint::~DDBPrint() { };

Packet *DDBPrint::simple_action(Packet *p) {
	struct DDBProto *proto = (DDBProto *) p->data();
	uint8_t anno_val = DDBPROTO_ANSWER + 1;

	if (ntohs(proto->T) == DDBPROTO_REQUEST) {
		click_chatter("Request: %s", String(proto->Data, DDBPROTO_DATA_LEN).c_str());
		anno_val = (uint8_t) DDBPROTO_REQUEST;
	} else if (ntohs(proto->T) == DDBPROTO_ANSWER) {
		click_chatter("Answer: %s", String(proto->Data, DDBPROTO_DATA_LEN).c_str());
		anno_val = (uint8_t) DDBPROTO_ANSWER;
	} else {
	  click_chatter("ERROR: unknown type for packet, proto-T: %u", proto->T);
	}

	p->set_anno_u8(DDB_CLASSIFY_ANNO_OFFSET, anno_val);
	return p;
};

CLICK_ENDDECLS
EXPORT_ELEMENT(DDBPrint)
