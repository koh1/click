#include <click/config.h>
#include <click/args.hh>
#include <clicknet/udp.h>
#include "DDBAnswer2.hh"
#include "ddbprotocol.hh"

CLICK_DECLS

DDBAnswer2::DDBAnswer2() { };
DDBAnswer2::~DDBAnswer2() { };

Packet *DDBAnswer2::simple_action(Packet *p) {
	const click_ip *iph_in = p->ip_header();
	struct in_addr dst = iph_in->ip_dst;
	struct in_addr src = iph_in->ip_src;
	// get 4 tuples
    click_chatter("DEBUG: source IP: %s", inet_ntoa(src));
    click_chatter("DEBUG: destination IP: %s", inet_ntoa(dst));

    const click_udp *udp_in = p->udp_header();
    click_chatter("DEBUG: source Port: %u", udp_in->uh_sport);
    click_chatter("DEBUG: destination Port: %u", udp_in->uh_dport);

	struct DDBProto *proto = (struct DDBProto*)p->data();
	String s = String(proto->Data, strnlen(proto->Data, DDBPROTO_DATA_LEN));
	click_chatter("RECEIVED: %d, %d, %s", proto->T, proto->Len, s.printable().c_str());
	//String res = _msgs.get(s);
	String res = _msgs.get(s);

	if (!res) {
		click_chatter("DEBUG: No response for %s", s.printable().c_str());
		p->kill();
		return NULL;
	}

	int res_len = res.length();
	res.append_fill('\0', DDBPROTO_DATA_LEN-res_len);

	struct DDBProto resp;
	resp.T = DDBPROTO_ANSWER;
	resp.Len = DDBPROTO_LEN_B;
	memcpy(resp.Data, res.c_str(), DDBPROTO_DATA_LEN);
	WritablePacket *q = Packet::make(headroom, &resp, sizeof(DDBProto), 0);
	p->kill();

	return q;
};

enum { H_MAP };

int DDBAnswer2::write_callback(const String &s, Element *e, void *vparam, ErrorHandler *errh) {
	DDBAnswer2 *da = static_cast<DDBAnswer2 *>(e);

	if ((intptr_t)vparam != H_MAP)
		return 0;

	if (s.length() > DDBPROTO_DATA_LEN*2 + 1)
		return 1;

	int pos_delim = s.find_left('|', 0);

	if (pos_delim == -1 || pos_delim == 0 || pos_delim == s.length())
		return 1;

	int pos_req = pos_delim > DDBPROTO_DATA_LEN ? DDBPROTO_DATA_LEN : pos_delim;

	String req = s.substring(0, pos_req);
	String ans = s.substring(pos_delim+1, s.length());

	click_chatter("Adding: '%s' -> '%s'", req.printable().c_str(), ans.printable().c_str());
	da->_msgs[req] = ans;
	return 0;

}

String DDBAnswer2::read_callback(Element *e, void *vparam) {
	DDBAnswer2 *da = static_cast<DDBAnswer2 *>(e);
	if ((intptr_t)vparam != H_MAP)
		return "";

	String res("");
	res += "------ MSGS ------\n";

	for (HashTable<String, String>::iterator it = da->_msgs.begin(); it; it++) {
		res += it.key().printable();
		res += " -> ";
		res += it.value().printable();
		res += "\n";
	}

	res += "------ END MSGS ------\n";
	return res;
}

void DDBAnswer2::add_handlers() {
	add_read_handler("h_map", read_callback, H_MAP, Handler::CALM);
	add_write_handler("h_map", write_callback, H_MAP);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DDBAnswer2)
