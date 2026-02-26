#include "stm32f10x.h"                  // Device header
#include "SysTick.h"


void systick_init(void)
{
	SysTick->CTRL |= 0b100;
	SysTick->LOAD = (SystemCoreClock / 0xF4240) - 1; // 1 MHz resolution | 1us precision
	SysTick->VAL = 0;
}


void delay_us(uint16_t us)
{
	SysTick->CTRL |= (1 << 0);
	for(uint32_t cus=0;cus<us;cus++)
		while(!(SysTick->CTRL & (1 << 16))); // COUNTFLAG bit resets by reading CTRL
	SysTick->CTRL &= ~(1 << 0);
}


void delay_ms(uint16_t ms)
{
	for(uint32_t cms=0; cms<ms; cms++)
		delay_us(1000);
}