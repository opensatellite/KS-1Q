#ifndef CBINARYPARSER_H
#define CBINARYPARSER_H

#include "binaryobjects.h"
#include "datatypes.h"
//#include <cstring.h>

/*
 * Convert messages from binary to ASCII
 * binary message format can be :
 *  -   msgUBX
 *  -   msgFDR
 *  -   msgRadio
 */

enum msgFormat { formatUBX, formatFDR, formatRadio, formatV2 };
enum msgState {
	stateHeader1,
	stateHeader2,
	stateClass,
	stateId,
	stateSeq,
	stateLength,
	stateChecker,
	stateContent,
	stateCkA,
	stateCkB
};

struct binaryParser
{
	//callbacks
	void (*sendbyte)(uint8_t);
	void (*sendcomplete)();
	bool (*selectpacket)(uint8_t msgClass);
	//void (*processpacket)(void);

	//public
	uint8_t msgClass;
	uint8_t msgId;
	uint8_t *buff_addr;
	int32_t size;

	//private
	enum msgFormat fmt;
	enum msgState  state;

	int32_t pos;
	int32_t size_pos;
	int32_t buff_size;
	uint8_t ckA, ckB;
	uint8_t ckARead, ckBRead;
	int32_t count;
	int32_t error;
	bool rxen;
};

void binaryParser_init(struct binaryParser *Parser, enum msgFormat Format, int32_t buff_size, uint8_t *buff_addr);
//send formatRadio
int sendPacket(struct binaryParser *Parser, uint8_t _class, uint8_t len, const uint8_t payload[]);
//v2 both for fdr and radio, no id, has seq + checker + timestamp, len 2byte, use crc16-ccitt, prefix 0xa57d, checker=seq+len(h8)+len(l8)
//int32_t (*sendPacket)(struct binaryParser *Parser, uint8_t _class, uint8_t len, const uint8_t payload[]);
int8_t parse(struct binaryParser *Parser, uint8_t data);//1 for new packet, 0 for success, -1 for error

#endif // CBINARYPARSER_H
