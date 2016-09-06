
#include "version.h"
#include "type.h"
#include "define.h"
#include "hw_gpio.h"
#include "hw_uart.h"
#include "hw_ds1820.h"
#include "hw_am2320.h"

#include "hw_relay.h"

#include "wdgHw.h"

#include "mainProcess.h"

static void gpio_intr_handler()
{
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);

	//m_printf("gpio_status:%X\n\r",gpio_status);

	if (gpio_status & BIT(0))// S_REED
	{
		debugf("irq00\n\r");
		//GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, (gpio_status & BIT(0)));
	}

	if(gpio_status & BIT(3))//
	{
    	debugf("irq03\n\r");
		//GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, (gpio_status & BIT(3)));
	}

	if (gpio_status & BIT(4))//
	{
 		debugf("irq04\n\r");
 		
 		_relay._changeStat();
 		
		//GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, (gpio_status & BIT(4)));
	}

	if (gpio_status & BIT(13))//
	{
		debugf("irq13\n\r");
		//GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, (gpio_status & BIT(13)));

		_relay._changeStat();
	}

	if (gpio_status & BIT(15))
	{
		debugf("irq15\n\r");
		//GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, (gpio_status & BIT(13)));
	}
}


void irom connectCb()
{
	debugf("I'm CONNECTED\n");
}

void init()
{
	_wdgHw._init();
	ds1820.init();

    ETS_GPIO_INTR_DISABLE();

	_gpio_init();

    ETS_GPIO_INTR_ATTACH((void*)gpio_intr_handler, NULL);

_uart0._conf(_BAUD_RATE_115200,_EIGHT_BITS,_TWO_STOP_BIT,_NONE_PARITY);

	// Config GPIO
	_gpio0._conf(_INTERRUPT,_PULL_NONE,_OFF,_INTR_POSEDGE);// S_REED
	//_gpio1._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);// TXD0
	_gpio2._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);//
	//_gpio3._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);// RXD0
	_gpioDtcNull._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);// DTC_NULL
	_gpio5._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);//
	_gpio12._conf(_INPUT,_PULL_NONE,_ON,_INTR_DISABLE);// TEMP-IO (DS1820)
	_gpioRelay._conf(_OUTPUT,_PULL_NONE,_OFF,_INTR_DISABLE);// CMD_RELAY
	_gpioAm2320._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);// TEMP-IO (AM2320)
	_gpio15._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);//
_gpioWdg._conf(_OUTPUT,_PULL_NONE,_ON,_INTR_DISABLE);// WDG_H + LED

	Serial.systemDebugOutput(true);

	_wdgHw._start();

	// _fileSys._mountFileSystem();

	// _fileSys._readCalandarFile();

	WifiAccessPoint.enable(false);
	WifiStation.enable(true);

	WifiStation.config(WIFI_SSID, WIFI_PWD);

#if IS_FIX_IP == 1
	IPAddress _ip;
	_ip = IPAddress(ADDR_IP_ESP);
	WifiStation.setIP(_ip);
	//WifiStation.enableDHCP(false);
#endif

	WifiStation.waitConnection(connectCb);

	int slot = rboot_get_current_rom();

	debugf("\n\n\n");
	debugf("\r\nVersion %s\r\n", VERSION);
	debugf("\r\nCurrently running rom %d.\r\n", slot);

	os_printf("restart system %s RADIATOR 0.0.0 %s\n\r",VERSION,WifiStation.getMAC().c_str());
	
	mainP._start();
}
