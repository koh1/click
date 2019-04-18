#include <click/config.h>
#include <click/glue.hh>

#include "DDBPrint.hh"
#include "ddbprotocol.hh"

CLICK_DECLS

DDBPrint::DDBPrint() { };
DDBPrint::~DDBPrint() { };

Packet *DDBPrint::simple_action(Packet *p) {
  click_chatter("packet received");
  
	struct DDBProto *proto = (DDBProto *) p->data();
	click_chatter("packet parsed %u, %u", proto->T, proto->Len);
	click_chatter("packet parsed %s", String(proto->Entry_ID, DDBPROTO_EID_LEN).c_str());	
	uint8_t anno_val = DDBPROTO_ANSWER + 1;
	click_chatter("packet parse OK");	
	if (proto->T == DDBPROTO_REQUEST) {
	  click_chatter("REQUEST");
		click_chatter("Request: %s", String(proto->Entry_ID, DDBPROTO_EID_LEN).c_str());
		anno_val = (uint8_t) DDBPROTO_REQUEST;
	} else if (proto->T == DDBPROTO_ANSWER) {
		click_chatter("Answer: %s", String(proto->Entry_ID, DDBPROTO_EID_LEN).c_str());
		anno_val = (uint8_t) DDBPROTO_ANSWER;
	} else if (proto->T == DDBPROTO_ANSWER_NOT_FOUND) {
		click_chatter("Answer Not Found: %s", String(proto->Entry_ID, DDBPROTO_EID_LEN).c_str());
		anno_val = (uint8_t) DDBPROTO_ANSWER_NOT_FOUND;
	} else {
	  click_chatter("ERROR: unknown type for packet, T: %u, Len: %u, Entry_ID+Data: %s", proto->T, proto->Len, String(proto->Entry_ID, DDBPROTO_EID_LEN).c_str());
	}

	p->set_anno_u8(DDB_CLASSIFY_ANNO_OFFSET, anno_val);
	return p;
};

CLICK_ENDDECLS
EXPORT_ELEMENT(DDBPrint)
