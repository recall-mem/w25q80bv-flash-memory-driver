#include "spi.h"


#define GPIOX					GPIOA
#define CSS_PIN				4


void spi1_init(SPI_CONFIG *conf)	// Init SPI1 as master
{
	cs_high(); // Set CS pin to idle
	
	SPI1->CR1 = 0;
	
	SPI1->CR1 |= (1 << 2);		// MSTR 1
	SPI1->CR1 |= (1 << 9);		// SSM 1
	SPI1->CR1 |= (1 << 8);		// SSI 1
	
	SPI1->CR1 |= (conf->mode << 0);					// CPOL CPHA
	SPI1->CR1 |= (conf->baudrate << 3);			// BR [2:0]
	SPI1->CR1 |= (conf->bitorder << 7);			// LSB FIRST bit

	SPI1->CR1 |= (1 << 6);	// SPE
}


void cs_high()
{
	// Change it if you use different pin
	// PA4 HIGH
	GPIOX->BSRR = (1 << CSS_PIN);
}


void cs_low()
{
	// Change it if you use different pin
	// PA4 LOW
	GPIOX->BSRR = (1 << (CSS_PIN + 16));
}


uint8_t	spi1_transmit(uint8_t data)
{
	while(!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR = data;
	while(!(SPI1->SR & SPI_SR_RXNE));
	return (uint8_t)SPI1->DR;
}


void spi1_transmit_msg(uint8_t *buffer, uint32_t size)
{
	for(uint32_t i=0; i<size; i++)	
	{
		while(!(SPI1->SR & SPI_SR_TXE));
		SPI1->DR = buffer[i];
		while(!(SPI1->SR & SPI_SR_RXNE));
		(void)SPI1->DR;
	}
}


void spi1_clear_ovr()
{
    (void)SPI1->DR;
    (void)SPI1->SR;
}

