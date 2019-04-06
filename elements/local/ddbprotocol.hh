/*
 * ddbprotocol.hh
 *
 *  Created on: Apr 7, 2019
 *      Author: koh1
 * 0       7       15      23      31
 * +---+---+---+---+---+---+---+---+
 * |T |LEN      |DATA              |
 */

#ifndef DDBPROTOCOL_HH_
#define DDBPROTOCOL_HH_



#define DDBPROTO_REQUEST 0
#define DDBPROTO_ANSWER 1
#define DDBPROTO_CACHE_REQUEST 2
#define DDBPROTO_CACHE_ANSWER 3

#define DDB_CLASSIFY_ANNO_OFFSET 4

#define DDBPROTO_DATA_LEN 100

#define DDBPROTO_LEN_A 1 << 0
#define DDBPROTO_LEN_B 1 << 1

struct DDBProto {
#if CLICK_BYTE_ORDER == CLICK_BIG_ENDIAN
	unsigned int T : 3;
	unsigned int Len : 10;
#elif CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
	unsigned int Len : 10;
	unsigned int T : 3;
#else
#error "Undefined Byte Order!"
#endif
	char Data[DDBPROTO_DATA_LEN];
} CLICK_SIZE_PACKED_ATTRIBUTE;

#endif /* DDBPROTOCOL_HH_ */
