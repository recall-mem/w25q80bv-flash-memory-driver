#ifndef W25Q80BV_INSTRUCTIONS_H
#define W25Q80BV_INSTRUCTIONS_H

#include <stdint.h>


// Erase, Program Instructions
#define W25Q80BV_CMD_WRITE_ENABLE					0x06u
#define W25Q80BV_CMD_WRITE_DISABLE				0x04u
#define W25Q80BV_CMD_WRITE_VSR						0x50u
#define W25Q80BV_CMD_READ_STATUS_REG1			0x05u	// (S7–S0)
#define W25Q80BV_CMD_READ_STATUS_REG2			0x35u	// (S15–S8)
#define W25Q80BV_CMD_WRITE_STATUS_REG			0x01u
#define W25Q80BV_CMD_PAGE_PROGRAM					0x02u
#define W25Q80BV_CMD_SECTOR_ERASE					0x20u
#define W25Q80BV_CMD_BLOCK_ERASE32K				0x52u
#define W25Q80BV_CMD_BLOCK_ERASE64K				0xD8u
#define W25Q80BV_CMD_CHIP_ERASE						0xC7u	// OR 0x60u same thing			
#define W25Q80BV_CMD_POWER_DOWN						0xB9u
#define W25Q80BV_CMD_CONTIN_READ_MODE			0xFFu
/*	These instructions left
	Erase / Program Suspend 75h
	Erase / Program Resume 7Ah
*/


// Read Instructions
#define W25Q80BV_CMD_READ_DATA						0x03u
#define W25Q80BV_CMD_FAST_READ						0x0Bu

/*	These instructions left
	Fast Read Dual Output 3Bh
	Fast Read Quad Output 6Bh
	Fast Read Dual I/O BBh
	Fast Read Quad I/O EBh
	Word Read Quad I/O E7h
	Octal Word Read Quad I/O E3h
	Set Burst with Wrap 77h
*/


// ID, Security Instructions
#define W25Q80BV_CMD_RELEASE_POWER_DOWN		0xABu
#define W25Q80BV_CMD_DEVICE_ID						0x90u
#define W25Q80BV_CMD_JEDEC_ID							0x9Fu
#define W25Q80BV_CMD_UNIQUE_ID						0x4Bu
#define W25Q80BV_CMD_READ_SFDP_REG				0x5Au
#define W25Q80BV_CMD_ERASE_SEC_REGS				0x44u
#define W25Q80BV_CMD_PROGRAM_SEC_REGS			0x42u
#define W25Q80BV_CMD_READ_SEC_REGS				0x48u
/*	These instructions left
	Manufacturer/Device ID by Dual I/O 92h
	Manufacturer/Device ID by Quad I/O 94h
*/


// #define W25Q80BV_CMD_

#endif