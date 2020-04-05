#ifndef CLICK_DDBCACHEPROTO_HH
#define CLICK_DDBCACHEPROTO_HH

#include <click/element.hh>
#include <click/hashtable.hh>

CLICK_DECLS

/*
=c

DDBCacheProto(TODO)

=s

TODO: Summary

=d

TODO: Complete description

*/

HashTable<String, String> _cache;

class DDBCacheProto : public Element {
	//TODO: Add private attributes


	public:
		DDBCacheProto();
		~DDBCacheProto();

		const char *class_name() const { return "DDBCacheProto"; }
		const char *port_count() const { return "2/2"; }
		const char *processing() const { return AGNOSTIC; }
		void push(int, Packet *);
		Packet *pull(int);

};

CLICK_ENDDECLS

#endif
