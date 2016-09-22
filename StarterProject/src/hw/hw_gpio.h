
#ifndef GPIO_INCLUDE__GPIO_H_
#define GPIO_INCLUDE__GPIO_H_

#define _gpioDtcNull	_gpio4
#define PIN_DS_1820 	12
#define _gpioRelay		_gpio13
#define _gpioAm2320 	_gpio14
#define _gpioWdg		_gpio16


#define NBRE_GPIO 17

//#define PERIPHS_IO_MUX			    	0x60000800
													//	function0	function1	function2	function3	function4
#define ADDR_GPIO0		(PERIPHS_IO_MUX + 0x34)		//	gpio0		spi_cs2
#define ADDR_GPIO1		(PERIPHS_IO_MUX + 0x18)		//	u0txd		spi_cs1					gpio1
#define ADDR_GPIO2		(PERIPHS_IO_MUX + 0x38)		//	gpio1					u1txd					u0txd
#define ADDR_GPIO3		(PERIPHS_IO_MUX + 0x14)		//	u0rxd								gpio3
#define ADDR_GPIO4		(PERIPHS_IO_MUX + 0x3C)		//	gpio4 // invert with GPIO5
#define ADDR_GPIO5		(PERIPHS_IO_MUX + 0x40)		//	gpio5 // invert with GPIO4
#define ADDR_GPIO6		(PERIPHS_IO_MUX + 0x1c)		//	sd_clk		spi_clk					gpio6
#define ADDR_GPIO7		(PERIPHS_IO_MUX + 0x20)		//	sd_data0	spi_miso				gpio7		u1txd
#define ADDR_GPIO8		(PERIPHS_IO_MUX + 0x24)		//	sd_data1	spi_mosi				gpio8		u1rxd
#define ADDR_GPIO9		(PERIPHS_IO_MUX + 0x28)		//	sd_data2	spi_hd					gpio9
#define ADDR_GPIO10		(PERIPHS_IO_MUX + 0x2c)		//	sd_data3	spi_wp					gpio10
#define ADDR_GPIO11		(PERIPHS_IO_MUX + 0x30)		//	sd_cmd		spi_cs0					gpio11
#define ADDR_GPIO12		(PERIPHS_IO_MUX + 0x04)		//	mtdi								gpio12
#define ADDR_GPIO13		(PERIPHS_IO_MUX + 0x08)		//	mtck								gpio13
#define ADDR_GPIO14		(PERIPHS_IO_MUX + 0x0C)		//	mtms								gpio14
#define ADDR_GPIO15		(PERIPHS_IO_MUX + 0x10)		//	mtdo								gpio15
#define ADDR_GPIO16		(PERIPHS_IO_MUX + 0x00)

#define _GPIO_PIN00			(0x60000328)
#define _GPIO_PIN04			(0x60000338)


//#define PERIPHS_IO_MUX_PULLUP           BIT7
#define PERIPHS_IO_MUX_PULLDWN          BIT6
//#define PERIPHS_IO_MUX_SLEEP_PULLUP     BIT3
#define PERIPHS_IO_MUX_SLEEP_PULLDWN    BIT2

#define _pullup_disable(PIN_NAME)              CLEAR_PERI_REG_MASK(PIN_NAME, PERIPHS_IO_MUX_PULLUP)
#define _pullup_enable(PIN_NAME)               SET_PERI_REG_MASK(PIN_NAME, PERIPHS_IO_MUX_PULLUP)

#define _pulldown_disable(PIN_NAME)             CLEAR_PERI_REG_MASK(PIN_NAME, PERIPHS_IO_MUX_PULLDWN)
#define _pullown_enable(PIN_NAME)              SET_PERI_REG_MASK(PIN_NAME, PERIPHS_IO_MUX_PULLDWN)

typedef enum {
	  _GPIO_0=0,	// GPIO0 		(function 0)
	  _GPIO_1=1,	// TX 			(function 0)
	  _GPIO_2=2,	// GPIO1 		(function 0)
	  _GPIO_3=3,	// RX 			(function 0)
	  _GPIO_4=4,	// GPIO4 		(function 0)
	  _GPIO_5=5,	// GPIO5 		(function 0)
	  _GPIO_6=6,	// SPI_CLK 		(function 1)
	  _GPIO_7=7,	// SPI_MISO 	(function 1)
	  _GPIO_8=8,	// SPI_MOSI 	(function 1)
	  _GPIO_9=9,	// SPI_HD 		(function 1)
	  _GPIO_10=10,	// SPI_WP 		(function 1)
	  _GPIO_11=11,	// SPI_CS0 		(function 1)
	  _GPIO_12=12,	// GPIO12		(function 3)
	  _GPIO_13=13,	// GPIO13		(function 3)
	  _GPIO_14=14,	// GPIO14		(function 3)
	  _GPIO_15=15,	// GPIO15		(function 3)
	  _GPIO_16=16
} _GPIO_Pin;

typedef enum {
  _FUNCTION_0=0,
  _FUNCTION_1=1,
  _FUNCTION_2=2,
  _FUNCTION_3=3,
  _FUNCTION_4=4,
  _FUNC_GPIO16=5
} _GPIO_PinFunct;


typedef enum {
    _INTR_DISABLE = 0,
    _INTR_POSEDGE = 1, // rising edge  ... front montant
    _INTR_NEGEDGE = 2, // falling edge ... front descendant
    _INTR_ANYEDGE = 3,
    _INTR_LOLEVEL = 4,
    _INTR_HILEVEL = 5
} _GPIO_intr_type;

typedef enum {
  _INPUT=0,
  _OUTPUT=1,
  _INTERRUPT=2,
} _GPIO_PinMode;

typedef enum {
  _OFF=0,
  _ON=1
} _GPIO_OutputStat;

typedef enum {
	  _PULL_NONE=0,
	  _PULL_UP=1,
	  _PULL_DOWN=2,
	  _PULL_BOTH=3
} _GPIO_PullStatus;

typedef struct{

	bool				gpio_use;
	_GPIO_PinFunct 		gpio_function;
	_GPIO_PinMode 		gpio_pinMode;
	_GPIO_PullStatus	gpio_pullStat;
	_GPIO_intr_type 	gpio_intrType;
	_GPIO_OutputStat 	gpio_outStat;

} _GPIO_gpio;

class _cGpio
{
private:

	uint8 		gpio_pin;

	_GPIO_gpio 	_gpio;


public:

	_cGpio(uint8 gpio_p);

	uint32 	_getAddr();
	bool 	_pull(_GPIO_PullStatus gpio_pullStat);
	bool 	_functionForGpio();

	int 	_conf(	_GPIO_PinMode gpio_pinMode,
					_GPIO_PullStatus gpio_pullStat,
					_GPIO_OutputStat gpio_outStat,
					_GPIO_intr_type gpio_intrType
					);

	uint8 	_inputGet();
	void 	_outputSet(_GPIO_OutputStat gpio_outStat);
	_GPIO_OutputStat _outputGet();
};


extern void _gpio_init();

extern _cGpio _gpio0;
extern _cGpio _gpio1;
extern _cGpio _gpio2;
extern _cGpio _gpio3;
extern _cGpio _gpio4;
extern _cGpio _gpio5;
extern _cGpio _gpio6;
extern _cGpio _gpio7;
extern _cGpio _gpio8;
extern _cGpio _gpio9;
extern _cGpio _gpio10;
extern _cGpio _gpio11;
extern _cGpio _gpio12;
extern _cGpio _gpio13;
extern _cGpio _gpio14;
extern _cGpio _gpio15;
extern _cGpio _gpio16;

#endif
