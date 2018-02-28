#include "cbinaryparser.h"
//#include <stdio.h>
//#include <QByteArray.h>

//////////////////////////////////////////////////////////////////////////

static uint8_t getHeader(struct binaryParser *Parser,int i)
{
	switch (Parser->fmt)
	{
	case formatFDR:
		if (i == 1) return 0xc5;
		else if (i == 2) return 0xa3;
		break;
	case formatRadio:
		if (i == 1) return 0xa5;
		else if (i == 2) return 0x7a;
		break;
	case formatUBX:
		if (i == 1) return 0xb5;
		else if (i == 2) return 0x62;
		break;
	case formatV2:
		if (i == 1) return 0xa5;
		else if (i == 2) return 0x7d;
		break;
	default:;
	}
	return 0;
}

static uint8_t getLengthSize(struct binaryParser *Parser)
{
	if (Parser->fmt == formatFDR || Parser->fmt == formatUBX || Parser->fmt == formatV2)
		return 2;
	else if (Parser->fmt == formatRadio)
		return 1;
	else return 1;
}

static bool classWithId(struct binaryParser *Parser)
{
	if (Parser->fmt == formatUBX)
		return true;
	else if (Parser->fmt == formatFDR && Parser->fmt == formatRadio && Parser->fmt == formatV2)
		return false;
	else return false;
}
/*
static bool hasSeq(struct binaryParser *Parser)
{
	if (Parser->fmt == formatV2) return true;
	else return false;
}

static bool hasChecker(struct binaryParser *Parser)
{
	if (Parser->fmt == formatV2) return true;
	else return false;
}

static bool isCRC(struct binaryParser *Parser)
{
	if (Parser->fmt == formatV2) return true;
	else return false;
}

static uint16_t crc16(uint8_t *data, int len)
{
	//return qChecksum((const char*)data, len);
  	return 0;
}
*/

int sendPacket(struct binaryParser *Parser,uint8_t _class, uint8_t len, const uint8_t payload[])
{
	if(!Parser->sendbyte) return 0;
	if (Parser->fmt != formatRadio) return 0;
	//send header, id, len, payload, chksum
	Parser->sendbyte(getHeader(Parser,1));
	Parser->sendbyte(getHeader(Parser,2));

	uint8_t ckA = 0, ckB = 0;
	Parser->sendbyte(_class);
	ckA += _class;
	ckB += ckA;
	Parser->sendbyte(len);
	ckA += len;
	ckB += ckA;
	for (int i = 0; i < len; i++)
	{
		Parser->sendbyte(payload[i]);
		ckA += payload[i];
		ckB += ckA;
	}
	Parser->sendbyte(ckA);
	Parser->sendbyte(ckB);
	if(Parser->sendcomplete)
            Parser->sendcomplete(Parser);
	return 2 + 1 + 1 + len + 2;
}

/*
int __sendPacket(uint8_t _class, uint8_t seq, uint32_t timestamp, uint8_t len, const uint8_t payload[])
{
	//TODO: send V2 packet
	if(fmt != formatV2) return 0;
	_class = _class;
	seq = seq;
	timestamp = timestamp;
	len = len;
	payload = payload;
	return 0;
}
*/

int8_t parse(struct binaryParser *Parser,uint8_t data)
{
	int8_t ret = 0;

	// read header 1
	if (Parser->state == stateHeader1)
	{
		if (data == getHeader(Parser,1))
		{
			Parser->state = stateHeader2;
		}
		else
		{
			++(Parser->error);
			ret = -1;
		}
	}
	// read header 2
	else if (Parser->state == stateHeader2)
	{
		if (data == getHeader(Parser,2))
		{
			// set up next state
			Parser->ckA = 0;
			Parser->ckB = 0;
			Parser->state = stateClass;
		}
		else if (data == getHeader(Parser,1))
		{
			// stay in this state
		}
		else
		{
			++(Parser->error);
			ret = -1;
			// go back to first state
			Parser->state = stateHeader1;
		}
	}
	// read the class of the message
	else if (Parser->state == stateClass)
	{
		// read class
		Parser->msgClass = data;
		Parser->rxen = Parser->selectpacket(Parser->msgClass);
		// checksum
		Parser->ckA += data;
		Parser->ckB += Parser->ckA;
		// go to next state: id
		if (classWithId(Parser))
			Parser->state = stateId;
		else
		{
			Parser->pos = 0;
			Parser->size_pos = 0;
			Parser->size = 0;
			Parser->state = stateLength;
		}
	}
	// read the id of the message
	else if (Parser->state == stateId)
	{
		// read id
		Parser->msgId = data;
		// checksum
		Parser->ckA += data;
		Parser->ckB += Parser->ckA;
		// set up next state: length
		Parser->pos = 0;
		Parser->size_pos = 0;
		Parser->size = 0;
		Parser->state = stateLength;
	}
	// read the size of the message
	else if (Parser->state == stateLength)
	{
		((uint8_t*)(&Parser->size))[(Parser->size_pos)++] = data;
		// checksum
		Parser->ckA += data;
		Parser->ckB += Parser->ckA;

		// size read completely
		if (Parser->size_pos == getLengthSize(Parser))
		{
			// select structure to fill
			Parser->state = stateContent;

			// reset byte count anyway
			Parser->pos = 0;
		}
	}
	// read the content of the message
	else if (Parser->state == stateContent)
	{
		// read byte and put it in the structure
		if (Parser->rxen)
		{
			(Parser->buff_addr)[Parser->pos++] = data;
		}

		// checksum
		Parser->ckA += data;
		Parser->ckB += Parser->ckA;

		// message is full, go to next state
		if (Parser->pos >= Parser->size || Parser->pos == Parser->buff_size)
		{
			Parser->state = stateCkA;
		}
	}
	// read the checksum byte A
	else if (Parser->state == stateCkA)
	{
		// read checksum A
		Parser->ckARead = data;
		// go to last state: checksum B
		Parser->state = stateCkB;
	}
	// read the checksum byte B
	else if (Parser->state == stateCkB)
	{
		// read checksum B
		Parser->ckBRead = data;

		// checksum is correct ?
		if (Parser->ckA == Parser->ckARead && Parser->ckB == Parser->ckBRead)
			//if(1)
		{
			++(Parser->count);
			if(Parser->rxen)
				ret = 1;
			//processPacket();
		}
		// checksum error
		else
		{
			++Parser->error;

			ret = -1;
			// keep same buffers as data is corrupted
		}
		// go back to state header1 for next message
		Parser->state = stateHeader1;
	}
	// major error, should never happen, the state is unknown !
	else
	{
		++(Parser->error);
		ret = -1;
		Parser->state = stateHeader1;
	}
	return ret;
}

static void sendbyte(uint8_t x)
{
	x = x;
}

static void sendcomplete(struct binaryParser *Parser)
{

}
/*
void processPacket(void)
{
	switch (fmt)
	{
	case formatFDR:

		break;
	case formatUBX:

		break;
	case formatRadio:

		break;
	default:;
	}
}
*/



static bool selectpacket(uint8_t _class)
{
	return false;
}

void binaryParser_init(struct binaryParser *Parser, enum msgFormat Format, int32_t buff_size, uint8_t *buff_addr)
{
	// user config
	Parser->fmt = Format;
	Parser->buff_size = buff_size;
	Parser->buff_addr = buff_addr;

	//default
	Parser->state = stateHeader1;
	Parser->pos = 0;
	Parser->count = 0;
	Parser->error = 0;

	Parser->sendbyte = &sendbyte;
	Parser->sendcomplete = &sendcomplete;
	Parser->selectpacket = &selectpacket;
	//binaryParser->processPacket = &processPacket;
}