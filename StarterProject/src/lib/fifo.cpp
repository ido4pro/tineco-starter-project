
#include "fifo.h"



irom _cFifo::_cFifo(uint8 num)
{
	numFifo = num;

	_idxIn = 0;
	_idxOut = 0;
	_idxNb = 0;
	_busy = false;
}

void irom _cFifo::_clean()
{
	_idxIn = 0;
	_idxOut = 0;
	_idxNb = 0;
	_busy = false;
}

uint16 irom _cFifo::_In(uint8 in)
{
	if(_busy == false)
	{
		_busy = true;

		_fifo[_idxIn++] = in;

		if(_idxIn >= LEN_FIFO)
			_idxIn = 0;

		if(_idxNb < LEN_FIFO)
			_idxNb ++;
		else
			_idxOut = _idxIn;

		_busy = false;
		return _FIFO_OK;
	}

	//debugf(" fb ");
	return _FIFO_BUSY;
}

uint16 irom _cFifo::_Out()
{
	if(_busy == false)
	{
		_busy = true;

		uint8 _c = 0;

		if(_idxNb > 0)
		{
			_c = _fifo[_idxOut++];

			if(_idxOut >= LEN_FIFO)
				_idxOut = 0;

			_idxNb--;

			_busy = false;
			return _c;
		}

	//debugf(" fe ");
		_busy = false;
		return _FIFO_EMPTY;
	}

//debugf(" fb ");
	return _FIFO_BUSY;
}

uint16 irom _cFifo::_len()
{
	if(_busy == false)
		return _idxNb;
	else
		return _FIFO_BUSY;
}

_cFifo _fifoIn(0);
//_cFifo _fifoOut(1);



