#include "main.h"
#include <stdlib.h>


#define PIN(pin)			\
				( (pin>7)			\
				? 4*(pin-8)		\
				: (4*pin) )

	
void init_clocks();
void init_pins();
void blink_led(uint8_t blinks);
	
	
uint8_t buffer[0x2000];	// 8kb
SPI_CONFIG conf = {
		.baudrate = SPI_BRDIV2,
		.bitorder = SPI_MSB_FIRST,
		.mode 		= SPI_MODE0
	};
	
	
	
int main()
{
	init_clocks();
	init_pins();
	
	uint8_t tx[4] = {0xDE, 0xAD, 0xBE, 0xEF};
	uint32_t addr = 0x000AC000;
	uint8_t ec = 0;
	
	ec = w25q_program_page(addr, tx, 4);
	
	if(!ec)	{
		w25q_read(addr, buffer, 0x4);
	}	else	{
		blink_led(3);
	}

	return 0;
}


void init_clocks()
{
	RCC->APB2ENR |= (1u << 0);		//AFIO clock enable
	RCC->APB2ENR |= (1u << 2);		//IOPA clock enable
	RCC->APB2ENR |= (1u << 12);		//SPI1 clock enable
}


void init_pins()
{
	// PA4 = CS, PA5 = SCK, PA6 = MISO, PA7 = MOSI
	GPIOA->CRL &= ~(0xF << PIN(4));
	GPIOA->CRL |= (0b0010 << PIN(4));
	
	GPIOA->CRL &= ~(0xF << PIN(5));
	GPIOA->CRL |= (0b1011 << PIN(5));
	
	GPIOA->CRL &= ~(0xF << PIN(6));
	GPIOA->CRL |= (0b0100 << PIN(6));
	
	GPIOA->CRL &= ~(0xF << PIN(7));
	GPIOA->CRL |= (0b1011 << PIN(7));
}


void blink_led(uint8_t blinks)
{
	RCC->APB2ENR |= (1 << 4); //enable IOPC clock
	
	//PC13 as output for integrated pin 
	GPIOC->CRH &= ~(0xF << 4*5);
	GPIOC->CRH |= (0b0011 << 4*5);
	GPIOC->BSRR = (1 << 13); //led off
	
	for(;blinks>0;blinks--) {
		GPIOC->BSRR = (1 << (13+16));
		for(volatile uint32_t t = 0; t<700000; t++);
		GPIOC->BSRR = (1 << 13);
		for(volatile uint32_t t = 0; t<700000; t++);
	}
	
	RCC->APB2ENR &= ~(1 << 4); //enable IOPC clock
}

