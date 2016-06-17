#ifndef _UART_APP_H
#define _UART_APP_H


#include <SmingCore/SmingCore.h>

#include "eagle_soc.h"


typedef enum {
    _FIVE_BITS = 0x0,
    _SIX_BITS = 0x1,
    _SEVEN_BITS = 0x2,
    _EIGHT_BITS = 0x3
} _UartNbrBits;

typedef enum {
    _ONE_STOP_BIT             = 0,
    _ONE_HALF_STOP_BIT        = BIT2,
    _TWO_STOP_BIT             = BIT2
} _UartStopBits;

typedef enum {
    _NONE_PARITY = 0,
    _ODD_PARITY   = 0,
    _EVEN_PARITY = BIT4
} _UartParity;

typedef enum {
    _PARITY_DIS   = 0,
    _PARITY_EN    = BIT3 | BIT5
} _UartExistParity;

typedef enum {
    _BAUD_RATE_1200     = 1200,
    _BAUD_RATE_2400     = 2400,
    _BAUD_RATE_4800     = 4800,
	_BAUD_RATE_9600     = 9600,
    _BAUD_RATE_19200    = 19200,
	_BAUD_RATE_115200 	= 115200,
} _UartBaudRate;

class _cUart
{
private:

	uint8 numUart;

public:

	_cUart(uint8 num);

	void _conf(_UartBaudRate _baudRate,_UartNbrBits _nbrBits,_UartStopBits _stopBits,_UartParity _parity);

	void _stopRxIrq();
	void _startRxIrq();
	void _restartRxIrq();
};

extern _cUart _uart0;

#endif

