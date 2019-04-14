/*
 * ddbprotocol.hh
 *
 *  Created on: Apr 7, 2019
 *      Author: koh1
 * 0       7       15      23      31
 * +---+---+---+---+---+---+---+---+
 * |T      |LEN    |DATA           |
 */

#ifndef DDBPROTOCOL_HH_
#define DDBPROTOCOL_HH_



#define DDBPROTO_REQUEST 0
#define DDBPROTO_ANSWER 1
#define DDBPROTO_ANSWER_NOT_FOUND 2
#define DDBPROTO_CACHE_ANSWER 3

#define DDB_CLASSIFY_ANNO_OFFSET 4

#define DDBPROTO_EID_LEN 9
#define DDBPROTO_HASH_LEN 36
#define DDBPROTO_DATA_LEN 255

#define DDBPROTO_LEN_A 1 << 0
#define DDBPROTO_LEN_B 1 << 1

struct DDBProto {
//#if CLICK_BYTE_ORDER == CLICK_BIG_ENDIAN
	unsigned int T : 8;
	unsigned int Len : 8;
//#elif CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN
//	unsigned int Len : 8;
//	unsigned int T : 8;
//#else
//#error "Undefined Byte Order!"
//#endif
    char Entry_ID[DDBPROTO_EID_LEN];
    char Hash[DDBPROTO_HASH_LEN];
	char Data[DDBPROTO_DATA_LEN-DDBPROTO_HASH_LEN-DDBPROTO_EID_LEN];
} CLICK_SIZE_PACKED_ATTRIBUTE;

#endif /* DDBPROTOCOL_HH_ */
