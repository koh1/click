/*
 * rfc2507c.{cc,hh} -- element compresses IP headers a la RFC 2507
 * Robert Morris
 *
 * Copyright (c) 1999 Massachusetts Institute of Technology.
 *
 * This software is being provided by the copyright holders under the GNU
 * General Public License, either version 2 or, at your discretion, any later
 * version. For more information, see the `COPYRIGHT' file in the source
 * distribution.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#ifdef __linux__
# define _BSD_SOURCE
#endif
#if defined(__linux__) && !defined(__KERNEL__)
extern "C" long int random(void);
#endif
#include "rfc2507c.hh"
#include <stdlib.h>

RFC2507c::RFC2507c()
{
  add_input();
  add_output();
}

RFC2507c::~RFC2507c()
{
}

RFC2507c *
RFC2507c::clone() const
{
  return(new RFC2507c());
}

/*
 * Make a packet containing an uncompressed and uncompressible
 * packet, not to be associated with a CID.
 */
Packet *
RFC2507c::make_other(Packet *p)
{
  Packet *q = Packet::make(p->length() + 1);
  q->data()[0] = PT_OTHER;
  memcpy(q->data() + 1, p->data(), p->length());
  return(q);
}

/*
 * Make a packet containing an uncompressed tcp/ip packet,
 * and also indicating a CID. The point is to cause the
 * decompressor to sync context with us.
 */
Packet *
RFC2507c::make_full(int cid, Packet *p)
{
  Packet *q = Packet::make(p->length() + 2);
  q->data()[0] = PT_FULL_HEADER;
  q->data()[1] = cid;
  memcpy(q->data() + 2, p->data(), p->length());

  return(q);
}

/*
 * Encode the difference between two fields (eg TCP seq #) in 0,
 * 1, or 2 bytes. If it won't fit in 2, return -1 signifying
 * that a full packet should be sent. If there is no difference,
 * return 0. If there is a difference, return 1.
 */
int
RFC2507c::encode16(int o, int n, char *p, int &i)
{
  return(encodeX(ntohs(o), ntohs(n), p, i));
}

int
RFC2507c::encode32(int o, int n, char *p, int &i)
{
  return(encodeX(ntohl(o), ntohl(n), p, i));
}

int
RFC2507c::encodeX(int o, int n, char *p, int &i)
{
  if(o == n)
    return(0);
  if(n < o)
    return(-1);
  if(n - o < 256){
    p[i++] = n - o;
    return(1);
  }
  if(n - o < 65536){
    p[i++] = 0;
    p[i++] = (n - o) >> 8;
    p[i++] = n - o;
    return(1);
  }
  return(-1);
}

Packet *
RFC2507c::make_compressed(int cid, Packet *p)
{
  int flags = 0; /* ROIPSAWU */
  int flen = 0;
  char fbuf[512]; /* encode into this buf[flen] */
  click_ip *ipp = (click_ip *) p->data();
  struct tcp_header *tcpp = (struct tcp_header *) (ipp + 1);
  int x;
  struct tcpip *ctx = &_ccbs[cid]._context;
  Packet *q = 0;

  if(ipp->ip_v != ctx->_ip.ip_v ||
     ipp->ip_hl != ctx->_ip.ip_hl ||
     ipp->ip_tos != ctx->_ip.ip_tos ||
     (ipp->ip_off & IP_DF) != (ctx->_ip.ip_off & IP_DF) ||
     ipp->ip_ttl != ctx->_ip.ip_ttl ||
     tcpp->th_off != ctx->_tcp.th_off){
    click_chatter("full1");
    goto full;
  }

  x = encode16(ctx->_tcp.th_urp, tcpp->th_urp, fbuf, flen);
  if(x < 0){
    click_chatter("full urp");
    goto full;
  }
  if(x)
    flags |= 1;

  x = encode16(ctx->_tcp.th_win, tcpp->th_win, fbuf, flen);
  if(x < 0){
    click_chatter("full win");
    goto full;
  }
  if(x)
    flags |= (1 << 1);

  x = encode32(ctx->_tcp.th_ack, tcpp->th_ack, fbuf, flen);
  if(x < 0){
    click_chatter("full ack");
    goto full;
  }
  if(x)
    flags |= (1 << 2);

  x = encode32(ctx->_tcp.th_seq, tcpp->th_seq, fbuf, flen);
  if(x < 0){
    click_chatter("full seq");
    goto full;
  }
  if(x)
    flags |= (1 << 3);

  if(ntohs(ipp->ip_id) != ntohs(ctx->_ip.ip_id) + 1){
    x = encode16(ctx->_ip.ip_id, ipp->ip_id, fbuf, flen);
    if(x < 0){
      click_chatter("full id");
      goto full;
    }
    if(x)
      flags |= (1 << 5);  
  }

  if(tcpp->th_flags & TH_PUSH)
    flags |= (1 << 4);
    
  q = Packet::make(p->length() -
		   sizeof(click_ip) - sizeof(struct tcp_header) +
		   5 + flen);
  q->data()[0] = PT_COMPRESSED_TCP;
  q->data()[1] = cid;
  q->data()[2] = flags;
  memcpy(q->data() + 3, &tcpp->th_sum, 2);
  memcpy(q->data() + 5, fbuf, flen);
  memcpy(q->data() + 5 + flen,
         p->data() + sizeof(click_ip) + sizeof(struct tcp_header),
         p->length() - sizeof(click_ip) - sizeof(struct tcp_header));
  return(q);

 full:
  q = make_full(cid, p);
  return(q);
}


/*
 * Zero out fields that should not participate in
 * identifying a flow.
 */
void
RFC2507c::make_key(const struct tcpip &from, struct tcpip &to)
{
  to = from;
  to._ip.ip_tos = 0;
  to._ip.ip_id = 0;
  to._ip.ip_len = 0;
  to._ip.ip_off = 0;
  to._ip.ip_ttl = 0;
  to._ip.ip_sum = 0;
  to._tcp.th_seq = 0;
  to._tcp.th_ack = 0;
  to._tcp.th_x2 = 0;
  to._tcp.th_off = 0;
  to._tcp.th_flags = 0;
  to._tcp.th_win = 0;
  to._tcp.th_sum = 0;
  to._tcp.th_urp = 0;
}

Packet *
RFC2507c::simple_action(Packet *p)
{
  click_ip *ipp = p->ip_header();
  assert(ipp && p->ip_header_offset() == 0);
  struct tcp_header *tcpp = (struct tcp_header *) (ipp + 1);
  int cid;
  Packet *q = 0;
  
  if(ipp->ip_hl != 5 ||
     ipp->ip_v != 4 ||
     (ntohs(ipp->ip_off) & (IP_OFFMASK | IP_MF)) != 0 ||
     ipp->ip_p != IPPROTO_TCP ||
     (tcpp->th_flags & (TH_FIN|TH_SYN|TH_RST|TH_ACK)) != TH_ACK){
    /* cannot compress this packet */
    click_chatter("cannot compress packet");
    q = make_other(p);
  } else {
    struct tcpip ti, key;
    ti._ip = *ipp;
    ti._tcp = *tcpp;
    make_key(ti, key);
    if(_map.findp(key) && (cid = _map[key])){
      /* sanity check */
      struct tcpip xkey;
      make_key(_ccbs[cid]._context, xkey);
      if(xkey != key){
        click_chatter("OOPS rfc2507c");
      }

      q = make_compressed(cid, p);
      _ccbs[cid]._context = ti;
    } else {
      /* pick a CID to re-use */
      cid = (random() % (TCP_SPACE-1)) + 1;

      /* delete the old key */
      struct tcpip okey;
      make_key(_ccbs[cid]._context, okey);
      if(_map.findp(okey) && _map[okey]){
        _map.insert(okey, 0);
      }

      _map.insert(key, cid);
      click_chatter("sport %d dport %d added cid %d",
                  ntohs(key._tcp.th_sport),
                  ntohs(key._tcp.th_dport),
                  cid);

      q = make_full(cid, p);
      _ccbs[cid]._context = ti;
    }
  }

  p->kill();
  return(q);
}

int
RFC2507c::tcpip::hashcode()
{
  unsigned int xx;
  xx = this->_ip.ip_src.s_addr +
    (this->_ip.ip_dst.s_addr << 1) +
    (this->_tcp.th_sport << 3) +
    (this->_tcp.th_dport << 4);
  return(xx & 0x7fffffff);
}

EXPORT_ELEMENT(RFC2507c)

#include "hashmap.cc"
template class HashMap<struct RFC2507c::tcpip, int>;
