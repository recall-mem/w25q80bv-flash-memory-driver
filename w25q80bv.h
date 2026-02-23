#ifndef W25Q_H
#define W25Q_H

#include "stm32f10x.h"                  // Device header
#include "spi.h"
#include <stdint.h>

#define W25Q_MEMORY_END              0x000FFF00h
#define W25Q80BV_MAX_PROGRAM_SIZE    0x100u 

#define W25Q80BV_ADDR_SEC_REG1			(uint32_t)0x001000	// to 0x0010FF
#define W25Q80BV_ADDR_SEC_REG2			(uint32_t)0x002000	// to 0x0020FF
#define W25Q80BV_ADDR_SEC_REG3			(uint32_t)0x003000	// to 0x0030FF



//////////////////////////
// Management functions //
//////////////////////////

void w25q_write_enable();
void w25q_power_down();
uint8_t w25q_release_power_down();
void w25q_reset();


////////////////////
// Read functions //
////////////////////

uint16_t w25q_read_status_reg();
void w25q_read(uint32_t addr, uint8_t *buffer, uint32_t length);
void w25q_read_security_reg(uint32_t addr, uint8_t *buffer, uint16_t length);
uint32_t w25q_read_jedec_id();


/////////////////////////////
// Write & Erase functions //
/////////////////////////////

uint8_t w25q_program_page(uint32_t addr, uint8_t *data, uint16_t length);
uint8_t w25q_program_sec_reg(uint32_t addr, uint8_t *data, uint16_t length);

/* Erases 4KB of memory starting from address */
void w25q_sector_erase(uint32_t addr); 
/* Erases 32KB of memory starting from address */
void w25q_block_erase_32k(uint32_t addr);
/* Erases 64KB of memory starting from address */
void w25q_block_erase_64k(uint32_t addr);
/* Erases all data in security register that in the given address */
void w25q_erase_sec_reg(uint32_t addr);


#endif 

