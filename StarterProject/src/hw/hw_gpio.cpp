
#include <SmingCore/SmingCore.h>

#include "type.h"
#include "hw_gpio.h"

static void irom gpio16_output_conf(void)
{
	WRITE_PERI_REG(PAD_XPD_DCDC_CONF,(READ_PERI_REG(PAD_XPD_DCDC_CONF) & 0xffffffbcUL) | 0x1UL); // mux configuration for XPD_DCDC to output rtc_gpio0
	WRITE_PERI_REG(RTC_GPIO_CONF,(READ_PERI_REG(RTC_GPIO_CONF) & 0xfffffffeUL) | 0x0UL); //mux configuration for out enable
	WRITE_PERI_REG(RTC_GPIO_ENABLE,(READ_PERI_REG(RTC_GPIO_ENABLE) & 0xfffffffeUL) | 0x1UL); //out enable
}

static void irom gpio16_input_conf(void)
{
	WRITE_PERI_REG(PAD_XPD_DCDC_CONF,(READ_PERI_REG(PAD_XPD_DCDC_CONF) & 0xffffffbcUL) | 0x1UL); // mux configuration for XPD_DCDC and rtc_gpio0 connection
	WRITE_PERI_REG(RTC_GPIO_CONF, (READ_PERI_REG(RTC_GPIO_CONF) & 0xfffffffeUL) | 0x0UL); //mux configuration for out enable
	WRITE_PERI_REG(RTC_GPIO_ENABLE,READ_PERI_REG(RTC_GPIO_ENABLE) & 0xfffffffeUL);  //out disable
}

void irom _gpio_init()
{

	// GPIO0 S_REED
	_gpio0._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);
	// GPIO1 TXD
//	.........
	// GPIO2 TEMP_IO
	_gpio2._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);
	// GPIO3 RXD / DETECT_PRESENCE
//	.........
	// GPIO4 IMP-IN / SWITCH_ADC
	_gpio4._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);
	// GPIO5 CMD_RELAY
	_gpio5._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);
	// GPIO6 SPI
//	.........
	// GPIO7 SPI
//	.........
	// GPIO8 SPI
//	.........
	// GPIO9 SPI
//	.........
	// GPIO10 SPI
//	.........
	// GPIO11 SPI
//	.........
	// GPIO12 WDG_LED
	_gpio12._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);
	// GPIO13 DETECT_ZERO
	_gpio13._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);
	// GPIO14 RESET_CRETE
	_gpio14._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);
	// GPIO15 C_OPEN
	_gpio15._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);
	// GPIO16 CONF_HDWR
	_gpio16._conf(_INPUT,_PULL_NONE,_OFF,_INTR_DISABLE);
}

irom _cGpio::_cGpio(uint8 gpio_p)
{
	gpio_pin = gpio_p;

	_gpio.gpio_use		= false;
	_gpio.gpio_function	= _FUNCTION_0;
	_gpio.gpio_pinMode	= _INPUT;
	_gpio.gpio_pullStat	= _PULL_NONE;
	_gpio.gpio_outStat	= _OFF;
	_gpio.gpio_intrType	= _INTR_DISABLE;
}

uint32 irom _cGpio::_getAddr()
{
	uint32 gpio_addr;
	switch(gpio_pin)
	{
	case _GPIO_0: 	gpio_addr = ADDR_GPIO0;		break;
	case _GPIO_1: 	gpio_addr = ADDR_GPIO1;		break;
	case _GPIO_2: 	gpio_addr = ADDR_GPIO2;		break;
	case _GPIO_3: 	gpio_addr = ADDR_GPIO3;		break;
	case _GPIO_4: 	gpio_addr = ADDR_GPIO4;		break;
	case _GPIO_5: 	gpio_addr = ADDR_GPIO5;		break;
	case _GPIO_6: 	gpio_addr = ADDR_GPIO6;		break;
	case _GPIO_7: 	gpio_addr = ADDR_GPIO7;		break;
	case _GPIO_8: 	gpio_addr = ADDR_GPIO8;		break;
	case _GPIO_9: 	gpio_addr = ADDR_GPIO9;		break;
	case _GPIO_10: 	gpio_addr = ADDR_GPIO10;	break;
	case _GPIO_11: 	gpio_addr = ADDR_GPIO11;	break;
	case _GPIO_12: 	gpio_addr = ADDR_GPIO12;	break;
	case _GPIO_13: 	gpio_addr = ADDR_GPIO13;	break;
	case _GPIO_14: 	gpio_addr = ADDR_GPIO14;	break;
	case _GPIO_15: 	gpio_addr = ADDR_GPIO15;	break;
	case _GPIO_16: 	gpio_addr = ADDR_GPIO16;	break;
	default:		gpio_addr = 0;				break;
	}

	return gpio_addr;
}

bool irom _cGpio::_functionForGpio()
{
	switch(gpio_pin)
	{
	case _GPIO_0: 	_gpio.gpio_function = _FUNCTION_0;	break;
	case _GPIO_1: 	_gpio.gpio_function = _FUNCTION_3;	break;
	case _GPIO_2: 	_gpio.gpio_function = _FUNCTION_0;	break;
	case _GPIO_3: 	_gpio.gpio_function = _FUNCTION_3;	break;
	case _GPIO_4: 	_gpio.gpio_function = _FUNCTION_0;	break;
	case _GPIO_5: 	_gpio.gpio_function = _FUNCTION_0;	break;
	case _GPIO_6: 	_gpio.gpio_function = _FUNCTION_3;	break;
	case _GPIO_7: 	_gpio.gpio_function = _FUNCTION_3;	break;
	case _GPIO_8: 	_gpio.gpio_function = _FUNCTION_3;	break;
	case _GPIO_9: 	_gpio.gpio_function = _FUNCTION_3;	break;
	case _GPIO_10: 	_gpio.gpio_function = _FUNCTION_3;	break;
	case _GPIO_11: 	_gpio.gpio_function = _FUNCTION_3;	break;
	case _GPIO_12: 	_gpio.gpio_function = _FUNCTION_3;	break;
	case _GPIO_13: 	_gpio.gpio_function = _FUNCTION_3;	break;
	case _GPIO_14: 	_gpio.gpio_function = _FUNCTION_3;	break;
	case _GPIO_15: 	_gpio.gpio_function = _FUNCTION_3;	break;
	case _GPIO_16: 	_gpio.gpio_function = _FUNC_GPIO16;	break;
	default: return false; break;
	}

	return true;
}

bool irom _cGpio::_pull(_GPIO_PullStatus gpio_pullStat)
{
	if(gpio_pin == _GPIO_16)
		return true;

	switch(gpio_pullStat)
	{
	case _PULL_NONE:
		GPIO_DIS_OUTPUT(gpio_pin);
		_pullup_disable		(_getAddr());
		_pulldown_disable	(_getAddr());
		break;
	case _PULL_UP:
		GPIO_DIS_OUTPUT(gpio_pin);
		_pullup_enable		(_getAddr());
		_pulldown_disable	(_getAddr());
		break;
	case _PULL_DOWN:
		GPIO_DIS_OUTPUT(gpio_pin);
		_pullup_disable		(_getAddr());
		_pullown_enable		(_getAddr());
		break;
	case _PULL_BOTH:
		GPIO_DIS_OUTPUT(gpio_pin);
		_pullup_enable		(_getAddr());
		_pullown_enable		(_getAddr());
		break;
	default:
		return false;
		break;
	}
	_gpio.gpio_pullStat	= gpio_pullStat;

	return true;
}

int irom _cGpio::_conf( _GPIO_PinMode gpio_pinMode,
						_GPIO_PullStatus gpio_pullStat,
						_GPIO_OutputStat gpio_outStat,
						_GPIO_intr_type gpio_intrType
						)
{
	if(gpio_pin > _GPIO_16)
		return -1;
	if(gpio_pinMode > _INTERRUPT)
		return -2;
	if(gpio_pullStat > _PULL_BOTH)
		return -3;
	if(gpio_outStat > _ON)
		return -4;
	if(gpio_intrType > _INTR_HILEVEL)
		return -5;

	if(_functionForGpio() == false)
		return -6;

	if(gpio_pin == _GPIO_16)
	{
		switch(gpio_pinMode)
		{
		case _INPUT:
			gpio16_input_conf();
			break;
		case _OUTPUT:
			gpio16_output_conf();
			GPIO_OUTPUT_SET(GPIO_ID_PIN(gpio_pin), gpio_outStat);
			break;
		default:
			return -8;
			break;
		}
	}
	else
	{
		switch(gpio_pinMode)
		{
		case _INPUT:
			PIN_FUNC_SELECT(_getAddr(), _gpio.gpio_function);
		    gpio_pin_intr_state_set(GPIO_ID_PIN(gpio_pin), (GPIO_INT_TYPE)_INTR_DISABLE);
			if(_pull(gpio_pullStat) == false)
				return -7;
			break;
		case _OUTPUT:
			PIN_FUNC_SELECT(_getAddr(), _gpio.gpio_function);
			if(_pull(gpio_pullStat) == false)
				return -7;
		    gpio_pin_intr_state_set(GPIO_ID_PIN(gpio_pin), (GPIO_INT_TYPE)_INTR_DISABLE);
			GPIO_OUTPUT_SET(GPIO_ID_PIN(gpio_pin), gpio_outStat);
			break;
		case _INTERRUPT:
			if(_pull(gpio_pullStat) == false)
				return -7;

		    PIN_FUNC_SELECT(_getAddr(), _gpio.gpio_function);
		    gpio_register_set(GPIO_PIN_ADDR(gpio_pin), GPIO_PIN_INT_TYPE_SET(_INTR_DISABLE)
		          | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE)
		          | GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

		    //clear gpio status
		    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(gpio_pin));

		    //enable interrupt
		    gpio_pin_intr_state_set(GPIO_ID_PIN(gpio_pin), (GPIO_INT_TYPE)gpio_intrType);
			break;
		default:
			return -9;
			break;
		}
	}

	_gpio.gpio_pinMode	= gpio_pinMode;
	_gpio.gpio_outStat	= gpio_outStat;
	_gpio.gpio_intrType	= gpio_intrType;

	_gpio.gpio_use 		= true;

	return 0;
}

void irom _cGpio::_outputSet(_GPIO_OutputStat gpio_outStat)
{
	if(_gpio.gpio_use != true)
		return;
	if(gpio_pin > _GPIO_16)
		return;
	if(_gpio.gpio_pinMode != _OUTPUT)
		return;
	if(gpio_outStat > _ON)
		return;

	if (gpio_pin == _GPIO_16)
	{
		WRITE_PERI_REG(RTC_GPIO_OUT,(READ_PERI_REG(RTC_GPIO_OUT) & 0xfffffffeUL) | (0x1UL & gpio_outStat));
	}
	else
	{
		GPIO_OUTPUT_SET(GPIO_ID_PIN(gpio_pin), gpio_outStat);
	}
	_gpio.gpio_outStat = gpio_outStat;
}

_GPIO_OutputStat irom _cGpio::_outputGet()
{
	if(_gpio.gpio_use != true)
		return _OFF;
	if(gpio_pin >= _GPIO_16)
		return _OFF;
	if(_gpio.gpio_pinMode != _OUTPUT)
		return _OFF;

	return _gpio.gpio_outStat;
}

uint8 irom _cGpio::_inputGet()
{
	if(_gpio.gpio_use != true)
		return 0;
	if(gpio_pin > _GPIO_16)
		return 0;
	if(_gpio.gpio_pinMode == _OUTPUT
	|| _gpio.gpio_pinMode > _INTERRUPT)
		return 0;

	if (gpio_pin == _GPIO_16)
	{
		return (READ_PERI_REG(RTC_GPIO_IN_DATA) & 1UL);
	}
	else
	{
		return GPIO_INPUT_GET(GPIO_ID_PIN(gpio_pin));
	}
}

_cGpio _gpio0(_GPIO_0);
_cGpio _gpio1(_GPIO_1);
_cGpio _gpio2(_GPIO_2);
_cGpio _gpio3(_GPIO_3);
_cGpio _gpio4(_GPIO_4);
_cGpio _gpio5(_GPIO_5);
_cGpio _gpio6(_GPIO_6);
_cGpio _gpio7(_GPIO_7);
_cGpio _gpio8(_GPIO_8);
_cGpio _gpio9(_GPIO_9);
_cGpio _gpio10(_GPIO_10);
_cGpio _gpio11(_GPIO_11);
_cGpio _gpio12(_GPIO_12);
_cGpio _gpio13(_GPIO_13);
_cGpio _gpio14(_GPIO_14);
_cGpio _gpio15(_GPIO_15);
_cGpio _gpio16(_GPIO_16);
