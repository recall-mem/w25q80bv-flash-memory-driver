#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>


void systick_init();

void delay_sc(uint16_t sc);		// Delay seconds
void delay_ms(uint16_t ms);		// Delay miliseconds
void delay_us(uint16_t us);		// Delay microseconds


#endif