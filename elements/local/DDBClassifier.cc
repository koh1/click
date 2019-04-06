#include <click/config.h>
#include "DDBClassifier.hh"
#include "ddbprotocol.hh"

CLICK_DECLS

DDBClassifier::DDBClassifier() { };
DDBClassifier::~DDBClassifier() { };

void DDBClassifier::push(int, Packet *p) {
	int out_port = 4;  // default output for junk

	if (p->anno_u8(DDB_CLASSIFY_ANNO_OFFSET) == DDBPROTO_REQUEST)
		out_port = 0;
	else if (p->anno_u8(DDB_CLASSIFY_ANNO_OFFSET) == DDBPROTO_ANSWER)
		out_port = 1;
	else if (p->anno_u8(DDB_CLASSIFY_ANNO_OFFSET) == DDBPROTO_CACHE_REQUEST)
		out_port = 2;
	else if (p->anno_u8(DDB_CLASSIFY_ANNO_OFFSET) == DDBPROTO_CACHE_ANSWER)
		out_port = 3;

	output(out_port).push(p);
}


CLICK_ENDDECLS
EXPORT_ELEMENT(DDBClassifier)
