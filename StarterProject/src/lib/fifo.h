#ifndef FIFO_H_
#define FIFO_H_

#include "type.h"

#define LEN_FIFO	1024

#define _FIFO_OK	0x0000
#define _FIFO_BUSY	0x1000
#define _FIFO_EMPTY	0x2000

class _cFifo
{
private:

	uint8 numFifo;

	uint8 	_fifo[LEN_FIFO];
	uint16 	_idxIn;
	uint16 	_idxOut;
	uint16 	_idxNb;

	bool	_busy;

public:

	_cFifo(uint8 num);

	void _clean();

	uint16 _In(uint8 in);
	uint16 _Out();

	uint16 _len();

};

extern _cFifo _fifoIn;
//extern _cFifo _fifoOut;

#endif
