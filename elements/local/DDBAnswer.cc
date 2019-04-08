#include <click/config.h>
#include <click/args.hh>
#include <clicknet/udp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "DDBAnswer.hh"
#include "ddbprotocol.hh"

CLICK_DECLS

DDBAnswer::DDBAnswer() {
	_id = 0;
};
DDBAnswer::~DDBAnswer() { };

void DDBAnswer::push(int, Packet *p) {
	const click_ip *iph_in = p->ip_header();
	struct in_addr dst = iph_in->ip_dst;
	struct in_addr src = iph_in->ip_src;
	uint16_t ipid = iph_in->ip_id;
	// get 4 tuples
	click_chatter("DEBUG: source IP: %s", inet_ntoa(src));
	click_chatter("DEBUG: destination IP: %s", inet_ntoa(dst));

	const click_udp *udp_in = p->udp_header();
	uint16_t sport = udp_in->uh_sport;
	uint16_t dport = udp_in->uh_dport;
	click_chatter("DEBUG: source Port: %u", ntohs(udp_in->uh_sport));
	click_chatter("DEBUG: destination Port: %u", ntohs(udp_in->uh_dport));

	struct DDBProto *proto = (struct DDBProto*)p->data();
	String s = String(proto->Data, strnlen(proto->Data, DDBPROTO_DATA_LEN));
	click_chatter("RECEIVED: %d, %d, %s", proto->T, proto->Len, s.printable().c_str());
	//String res = _msgs.get(s);
	String res = _msgs.get(s);
	if (!res) {
		click_chatter("DEBUG: No response for %s", s.printable().c_str());
		p->push(28);
		output(1).push(p);
		return;
	}

	int res_len = res.length();
	res.append_fill('\0', DDBPROTO_DATA_LEN-res_len);

	struct DDBProto resp;
	resp.T = DDBPROTO_ANSWER;
	resp.Len = DDBPROTO_LEN_B;
	memcpy(resp.Data, res.c_str(), DDBPROTO_DATA_LEN);
	WritablePacket *q = Packet::make(headroom, &resp, sizeof(DDBProto), 0);
	q->push(sizeof(click_udp) + sizeof(click_ip));
	click_ip *ip = reinterpret_cast<click_ip *>(q->data());
	click_udp *udp = reinterpret_cast<click_udp *>(ip + 1);

	ip->ip_v = 4;
	ip->ip_hl = sizeof(click_ip) >> 2;
	ip->ip_len = htons(q->length());
	ip->ip_id = htons(_id.fetch_and_add(1));
	ip->ip_p = IP_PROTO_UDP;
	ip->ip_src = dst;
	ip->ip_dst = src;
	q->set_dst_ip_anno(IPAddress(src));

	ip->ip_tos = 0;
	ip->ip_off = 0;
	ip->ip_ttl = 250;

	ip->ip_sum = 0;
	ip->ip_sum = click_in_cksum((unsigned char *)ip, sizeof(click_ip));

	q->set_ip_header(ip, sizeof(click_ip));

	// set up UDP header
	udp->uh_sport = dport;
	udp->uh_dport = sport;
	uint16_t len = q->length() - sizeof(click_ip);
	udp->uh_ulen = htons(len);
	udp->uh_sum = 0;
	unsigned csum = click_in_cksum((unsigned char *)udp, len);
	udp->uh_sum = click_in_cksum_pseudohdr(csum, ip, len);

	p->kill();
	output(0).push(q);
	return;
};

enum { H_MAP };

int DDBAnswer::write_callback(const String &s, Element *e, void *vparam, ErrorHandler *errh) {
	DDBAnswer *da = static_cast<DDBAnswer *>(e);

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

String DDBAnswer::read_callback(Element *e, void *vparam) {
	DDBAnswer *da = static_cast<DDBAnswer *>(e);
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

void DDBAnswer::add_handlers() {
	add_read_handler("h_map", read_callback, H_MAP, Handler::CALM);
	add_write_handler("h_map", write_callback, H_MAP);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DDBAnswer)
