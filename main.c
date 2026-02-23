#include "main.h"
#include <stdlib.h>


#define PIN(pin)			\
				( (pin>7)			\
				? 4*(pin-8)		\
				: (4*pin) )



SPI_CONFIG conf = {
		.baudrate = SPI_BRDIV2,
		.bitorder = SPI_MSB_FIRST,
		.mode 		= SPI_MODE0
	};

	
void init_clocks();
void init_pins();

	
uint8_t buffer[0x2000];	//8 KB 
	

int main()
{
	init_clocks();
	init_pins();
	spi1_init(&conf);
	
	
	uint32_t addr = 0x00000000u;
	uint8_t read_counter = 0;
	
	while(1)	{
		w25q_read(addr, buffer, 0x2000);
		read_counter++;
		addr += 0x2000;
	}

	while(1) {}; // Hang here

		
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

