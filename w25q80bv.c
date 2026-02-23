#include "w25q80bv.h"
#include "w25q80bv_instructions.h"


#define ROUGH_DELAY(x) do { for(volatile int i=0; i<x; i++); } while(0)

// USER CONFIGURATION
// Add your header and change ROUGH_DELAY(us) to your delay function.
// The delay function must have microsecond precision!
// Function must accept microseconds as argument.
#define W25Q_DELAY_US(us)	ROUGH_DELAY(us)

#define DUMMY_BYTE			(uint8_t)0xFF


void w25q_write_enable()
{
	cs_low();
	
	(void)spi1_transmit(W25Q80BV_CMD_WRITE_ENABLE);
	
	cs_high();
}


void w25q_power_down()
{
	cs_low();
	
	(void)spi1_transmit(W25Q80BV_CMD_POWER_DOWN);
	
	cs_high();
}


uint8_t w25q_release_power_down()
{
	cs_low();
	
	(void)spi1_transmit(W25Q80BV_CMD_RELEASE_POWER_DOWN);
	(void)spi1_transmit(DUMMY_BYTE);
	(void)spi1_transmit(DUMMY_BYTE);
	(void)spi1_transmit(DUMMY_BYTE);
	
	uint8_t ID = spi1_transmit(DUMMY_BYTE);
	
	cs_high();
	
	W25Q_DELAY_US(10);	// 3 us required
	
	return ID;
}


void w25q_reset()
{
	cs_low();
	
	(void)spi1_transmit(0x66);	// Enable reset
		
	cs_high();
	
	cs_low();
	
	(void)spi1_transmit(0x99);	// Reset device

	cs_high();

	W25Q_DELAY_US(30);	// 30us required
}


uint16_t w25q_read_status_reg()
{
	uint8_t data[2];
	
	cs_low();
	
	(void)spi1_transmit(W25Q80BV_CMD_READ_STATUS_REG1);
	data[0] = spi1_transmit(DUMMY_BYTE);
	
	cs_high();
	
	cs_low();
	
	(void)spi1_transmit(W25Q80BV_CMD_READ_STATUS_REG2);
	data[1] = spi1_transmit(DUMMY_BYTE);
	
	cs_high();
	
	
	return (uint16_t)(((uint16_t)data[1] << 8) | data[0]);
}


void w25q_read_security_reg(uint32_t addr, uint8_t *buffer, uint16_t length)
{
	cs_low();

	(void)spi1_transmit(W25Q80BV_CMD_READ_SEC_REGS);
	
	(void)spi1_transmit((uint8_t)((addr >> 16)& 0xFF));	// A23-A16			
	(void)spi1_transmit((uint8_t)((addr >> 8)& 0xFF));	// A15-A8
	(void)spi1_transmit((uint8_t)(addr & 0xFF));				//A7-A0
	
	(void)spi1_transmit(DUMMY_BYTE);
	
	for(uint16_t i=0; i<length; i++)
	{
		buffer[i] = spi1_transmit(DUMMY_BYTE);
	}
	
	cs_high();
}


void w25q_read(uint32_t addr, uint8_t *buffer, uint32_t length)
{
	cs_low();
	
	(void)spi1_transmit(W25Q80BV_CMD_READ_DATA);
	
	(void)spi1_transmit((uint8_t)((addr >> 16)& 0xFF));	// A23-A16			
	(void)spi1_transmit((uint8_t)((addr >> 8)& 0xFF));	// A15-A8
	(void)spi1_transmit((uint8_t)(addr & 0xFF));				//A7-A0
	
	for(uint32_t i=0; i<length; i++)
	{
		buffer[i] = spi1_transmit(DUMMY_BYTE);
	}
	
	cs_high();
}


uint32_t w25q_read_jedec_id()
{
	uint8_t data[3];
	
	cs_low();
	
	(void)spi1_transmit(W25Q80BV_CMD_JEDEC_ID);
	
	data[0] = spi1_transmit(DUMMY_BYTE);		// (MF7-MF0) Manufacturer
	data[1] = spi1_transmit(DUMMY_BYTE);		// (ID15-ID8)	Memory Type
	data[2] = spi1_transmit(DUMMY_BYTE);		// (ID7-ID0) Capacity
	
	cs_high();
	
	return (uint32_t)(((uint32_t)data[0] << 16) | ((uint32_t)data[1] << 8) | data[2]);
}


/* Erases 4KB of memory starting from address */
void w25q_sector_erase(uint32_t addr)
{
	w25q_write_enable();
	
	cs_low();
	
	(void)spi1_transmit(W25Q80BV_CMD_SECTOR_ERASE);
	
	(void)spi1_transmit((uint8_t)((addr >> 16)& 0xFF));	// A23-A16			
	(void)spi1_transmit((uint8_t)((addr >> 8)& 0xFF));	// A15-A8
	(void)spi1_transmit((uint8_t)(addr & 0xFF));				//A7-A0
	
	cs_high();
	
	// wait utill BSY bit reset
	while(w25q_read_status_reg() & 1);
}


/* Erases 32KB of memory starting from address */
void w25q_block_erase_32k(uint32_t addr)
{
	w25q_write_enable();
	
	cs_low();
	
	(void)spi1_transmit(W25Q80BV_CMD_BLOCK_ERASE32K);
	
	(void)spi1_transmit((uint8_t)((addr >> 16)& 0xFF));	// A23-A16			
	(void)spi1_transmit((uint8_t)((addr >> 8)& 0xFF));	// A15-A8
	(void)spi1_transmit((uint8_t)(addr & 0xFF));				//A7-A0
	
	cs_high();
	
	// wait utill BSY bit reset
	while(w25q_read_status_reg() & 1);
}


/* Erases 64KB of memory starting from address */
void w25q_block_erase_64k(uint32_t addr)
{
	w25q_write_enable();
	
	cs_low();
	
	(void)spi1_transmit(W25Q80BV_CMD_BLOCK_ERASE64K);
	
	(void)spi1_transmit((uint8_t)((addr >> 16)& 0xFF));	// A23-A16			
	(void)spi1_transmit((uint8_t)((addr >> 8)& 0xFF));	// A15-A8
	(void)spi1_transmit((uint8_t)(addr & 0xFF));				//A7-A0
	
	cs_high();
	
	// wait utill BSY bit reset
	while(w25q_read_status_reg() & 1);
}


void w25q_erase_sec_reg(uint32_t addr)
{
	w25q_write_enable();
	
	cs_low();
	
	(void)spi1_transmit(W25Q80BV_CMD_ERASE_SEC_REGS);
	
	(void)spi1_transmit((uint8_t)((addr >> 16)& 0xFF));	// A23-A16			
	(void)spi1_transmit((uint8_t)((addr >> 8)& 0xFF));	// A15-A8
	(void)spi1_transmit((uint8_t)(addr & 0xFF));				//A7-A0
	
	cs_high();
	
	// wait utill BSY bit reset
	while(w25q_read_status_reg() & 1);
}

uint8_t w25q_program_page(uint32_t addr, uint8_t *data, uint16_t length)
{
	
	if(((addr & 0xFF) + length) > W25Q80BV_MAX_PROGRAM_SIZE)	{
		// The data will not fit in remaning space of page
		return 1; 
	}
	
	w25q_write_enable();
	
	cs_low();
	
	(void)spi1_transmit(W25Q80BV_CMD_PAGE_PROGRAM);
	
	// Sending the starting address for writing
	(void)spi1_transmit((uint8_t)((addr >> 16)& 0xFF));	// A23-A16			
	(void)spi1_transmit((uint8_t)((addr >> 8)& 0xFF));	// A15-A8
	(void)spi1_transmit((uint8_t)(addr & 0xFF));				//A7-A0
	
	for(uint16_t i=0; i<length; i++)	{
		(void)spi1_transmit(data[i]);
	}
	
	cs_high();
	
	// wait utill BSY bit reset
	while(w25q_read_status_reg() & 1);
	
	return 0; // :)
}


uint8_t w25q_program_sec_reg(uint32_t addr, uint8_t *data, uint16_t length)
{
	
	if(((addr & 0xFF) + length) > W25Q80BV_MAX_PROGRAM_SIZE)	{
		// The data will not fit in remaning space of register
		return 1; 
	}
	
	w25q_write_enable();
	
	cs_low();
	
	(void)spi1_transmit(W25Q80BV_CMD_PROGRAM_SEC_REGS);
	
	// Sending the starting address for writing
	(void)spi1_transmit((uint8_t)((addr >> 16)& 0xFF));	// A23-A16			
	(void)spi1_transmit((uint8_t)((addr >> 8)& 0xFF));	// A15-A8
	(void)spi1_transmit((uint8_t)(addr & 0xFF));				//A7-A0
	
	for(uint16_t i=0; i<length; i++)
	{
		(void)spi1_transmit(data[i]);
	}
	
	cs_high();
	
	// wait utill BSY bit reset
	while(w25q_read_status_reg() & 1);
	
	return 0; // :)
}

