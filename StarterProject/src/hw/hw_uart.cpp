#include "fifo.h"
#include "hw_uart.h"

#define 	uart_recvTaskPrio        1
#define 	uart_recvTaskQueueLen    10
os_event_t  uart_recvTaskQueue[uart_recvTaskQueueLen];

#define DBG  //os_printf
#define DBG1 os_printf//uart1_sendStr_no_wait
#define DBG2 os_printf

LOCAL void uart_rx_intr_handler(void *para);
LOCAL void uart_recvTask(os_event_t *events);

LOCAL void uart_rx_intr_handler(void *para)
{
    if(UART_FRM_ERR_INT_ST == (READ_PERI_REG(UART_INT_ST(0)) & UART_FRM_ERR_INT_ST))
    {
        DBG1("FRM_ERR\r\n");
        WRITE_PERI_REG(UART_INT_CLR(0), UART_FRM_ERR_INT_CLR);
    }
    else
    if(UART_RXFIFO_FULL_INT_ST == (READ_PERI_REG(UART_INT_ST(0)) & UART_RXFIFO_FULL_INT_ST))
    {
        DBG("f-");
        CLEAR_PERI_REG_MASK(UART_INT_ENA(0), UART_RXFIFO_FULL_INT_ENA|UART_RXFIFO_TOUT_INT_ENA);
        WRITE_PERI_REG(UART_INT_CLR(0), UART_RXFIFO_FULL_INT_CLR);
        system_os_post(uart_recvTaskPrio, 0, 0);
    }
    else
    if(UART_RXFIFO_TOUT_INT_ST == (READ_PERI_REG(UART_INT_ST(0)) & UART_RXFIFO_TOUT_INT_ST))
    {
        DBG("t-");
        CLEAR_PERI_REG_MASK(UART_INT_ENA(0), UART_RXFIFO_FULL_INT_ENA|UART_RXFIFO_TOUT_INT_ENA);
        WRITE_PERI_REG(UART_INT_CLR(0), UART_RXFIFO_TOUT_INT_CLR);
        system_os_post(uart_recvTaskPrio, 0, 0);
    }
    else
    if(UART_TXFIFO_EMPTY_INT_ST == (READ_PERI_REG(UART_INT_ST(0)) & UART_TXFIFO_EMPTY_INT_ST))
    {
        DBG("e-");
        CLEAR_PERI_REG_MASK(UART_INT_ENA(0), UART_TXFIFO_EMPTY_INT_ENA);
        system_os_post(uart_recvTaskPrio, 1, 0);
        WRITE_PERI_REG(UART_INT_CLR(0), UART_TXFIFO_EMPTY_INT_CLR);

    }
    else
    if(UART_RXFIFO_OVF_INT_ST  == (READ_PERI_REG(UART_INT_ST(0)) & UART_RXFIFO_OVF_INT_ST))
    {
        CLEAR_PERI_REG_MASK(UART_INT_ENA(0), UART_RXFIFO_FULL_INT_ENA|UART_RXFIFO_TOUT_INT_ENA);
        WRITE_PERI_REG(UART_INT_CLR(0), UART_RXFIFO_OVF_INT_CLR);
        DBG1("RX OVF!!\r\n");
    	WRITE_PERI_REG(UART_INT_CLR(0), 0xffff);
        SET_PERI_REG_MASK(UART_INT_ENA(0), UART_RXFIFO_FULL_INT_ENA|UART_RXFIFO_TOUT_INT_ENA);
    }
}

LOCAL void irom uart_recvTask(os_event_t *events)
{
	DBG("tsk ");

    if(events->sig == 0)
    {
/*        uint8 fifo_len = (READ_PERI_REG(UART_STATUS(0))>>UART_RXFIFO_CNT_S)&UART_RXFIFO_CNT;
        uint8 d_tmp = 0;
        uint8 idx=0;
        for(idx=0;idx<fifo_len;idx++)
        {
            d_tmp = READ_PERI_REG(UART_FIFO(0)) & 0xFF;

			_fifoIn._In(d_tmp);
        }*/
    	while((READ_PERI_REG(UART_STATUS(0))>>UART_RXFIFO_CNT_S)&UART_RXFIFO_CNT)
    		_fifoIn._In(READ_PERI_REG(UART_FIFO(0)) & 0xFF);

        WRITE_PERI_REG(UART_INT_CLR(0), UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);
        SET_PERI_REG_MASK(UART_INT_ENA(0), UART_RXFIFO_FULL_INT_ENA|UART_RXFIFO_TOUT_INT_ENA);
    }

    if(events->sig == 1)
    {
    	DBG1("TXFIFO_EMPTY\r\n");
    }
}

irom _cUart::_cUart(uint8 num)
{
	numUart = num;
}

void irom _cUart::_conf(_UartBaudRate _baudRate,_UartNbrBits _nbrBits,_UartStopBits _stopBits,_UartParity _parity)
{
	_UartExistParity existParity = _PARITY_DIS;

	ETS_UART_INTR_DISABLE();

    system_os_task(uart_recvTask, uart_recvTaskPrio, uart_recvTaskQueue, uart_recvTaskQueueLen);  //demo with a task to process the uart data

	ETS_UART_INTR_ATTACH((void*)uart_rx_intr_handler, NULL);

	PIN_PULLUP_DIS(PERIPHS_IO_MUX_U0TXD_U);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD);

	uart_div_modify(numUart, UART_CLK_FREQ / _baudRate);

	if(_parity == _ODD_PARITY || _parity == _EVEN_PARITY)
		existParity = _PARITY_EN;

	WRITE_PERI_REG(UART_CONF0(numUart), existParity | _parity | (_stopBits << UART_STOP_BIT_NUM_S) | (_nbrBits << UART_BIT_NUM_S));

	//clear rx and tx fifo,not ready
	SET_PERI_REG_MASK(UART_CONF0(numUart), UART_RXFIFO_RST | UART_TXFIFO_RST);
	CLEAR_PERI_REG_MASK(UART_CONF0(numUart), UART_RXFIFO_RST | UART_TXFIFO_RST);

    //set rx fifo trigger
		WRITE_PERI_REG(UART_CONF1(numUart),
					   ((0x10 & UART_RXFIFO_FULL_THRHD) << UART_RXFIFO_FULL_THRHD_S) |
					   ((0x10 & UART_RX_FLOW_THRHD) << UART_RX_FLOW_THRHD_S) |
					   UART_RX_FLOW_EN |
					   (0x02 & UART_RX_TOUT_THRHD) << UART_RX_TOUT_THRHD_S |
					   UART_RX_TOUT_EN);

	//clear all interrupt
	WRITE_PERI_REG(UART_INT_CLR(numUart), 0xffff);
}

void irom _cUart::_restartRxIrq()
{
	ETS_UART_INTR_DISABLE();

	WRITE_PERI_REG(UART_INT_CLR(numUart), 0xffff);

	WRITE_PERI_REG(UART_INT_ENA(numUart), UART_RXFIFO_TOUT_INT_ENA | UART_RXFIFO_FULL_INT_ENA);

	ETS_UART_INTR_ENABLE();
}

void irom _cUart::_stopRxIrq()
{
	ETS_UART_INTR_DISABLE();

	WRITE_PERI_REG(UART_INT_CLR(numUart), 0xffff);
}

void irom _cUart::_startRxIrq()
{
	WRITE_PERI_REG(UART_INT_ENA(numUart), UART_RXFIFO_TOUT_INT_ENA | UART_RXFIFO_FULL_INT_ENA);

	ETS_UART_INTR_ENABLE();
}

_cUart _uart0(0);
