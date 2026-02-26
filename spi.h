#ifndef SPI_H
#define SPI_H

#include "stm32f10x.h"                  // Device header
#include <stdint.h>


#define GPIOX					GPIOA
#define CSS_PIN				4


typedef enum {
		SPI_BRDIV2 = 0,
		SPI_BRDIV4,
		SPI_BRDIV8,
		SPI_BRDIV16,
		SPI_BRDIV32,
		SPI_BRDIV64,
		SPI_BRDIV128,
		SPI_BRDIV256
} SPI_BRATEDIVn;

typedef enum {
		SPI_MODE0 = 0, // CPOL=0, CPHA=0	(idle CLK=LOW)
		SPI_MODE1, // CPOL=0, CPHA=1	(idle CLK=LOW)
		SPI_MODE2, // CPOL=1, CPHA=0	(idle CLK=HIGH)
		SPI_MODE3, // CPOL=1, CPHA=1	(idle CLK=HIGH)
} SPI_MODEn;

typedef enum {
		SPI_MSB_FIRST = 0,
		SPI_LSB_FIRST
} SPI_BIT_ORDER;

typedef struct {
		SPI_MODEn				mode;
		SPI_BRATEDIVn 	baudrate;
		SPI_BIT_ORDER 	bitorder;
} SPI_CONFIG;


void spi1_init(SPI_CONFIG *conf);

uint8_t spi1_transmit(uint8_t data);
void spi1_transmit_msg(uint8_t *buffer, uint32_t size);

void spi1_clear_ovr();

inline void cs_high()
{
	// Change it if you use different pin
	// PA4 HIGH
	GPIOX->BSRR = (1 << CSS_PIN);
}


inline void cs_low()
{
	// Change it if you use different pin
	// PA4 LOW
	GPIOX->BSRR = (1 << (CSS_PIN + 16));
}


#endif