# W25Q80BV Flash Memory Driver

A lightweight, bare-metal SPI driver for the **Winbond W25Q80BV** 8Mbit serial flash memory, written in C for the **STM32F103CB** microcontroller (Blue Pill). The driver is written at the register level without HAL dependencies.

---

## Hardware

### Pin Connections (SPI1)

| W25Q80BV Pin | STM32F103C8 Pin | Function       |
|:------------:|:---------------:|:--------------:|
| CS           | PA4             | Chip Select    |
| CLK          | PA5             | SPI Clock      |
| DO (MISO)    | PA6             | Master In      |
| DI (MOSI)    | PA7             | Master Out     |
| VCC          | 3.3V            | Power          |
| GND          | GND             | Ground         |

> **Note:** CS pin and GPIO port are configurable in `spi.c`. Modify `CSS_PIN` and `GPIOX` defines at the top of the file.

---

## User Configuration

Before using the driver, two things must be configured in `w25q80bv.c`:

### 1. Delay Function

The driver uses a macro for time-sensitive delays (e.g. after power-down release and reset). By default, a rough busy-wait loop is used:

```c
#define ROUGH_DELAY(x)    do { for(volatile int i=0; i<x; i++); } while(0)
#define W25Q_DELAY_US(us) ROUGH_DELAY(us)
```

**It is strongly recommended to replace this with a proper microsecond delay function** from your system (e.g. SysTick-based, HAL, or RTOS):

```c
// Add your delay header
#include "my_delay.h"

// Replace the macro
#define W25Q_DELAY_US(us) delay_us(us)
```

The function must accept microseconds as its argument: `void delay_us(uint32_t us)`.

### 2. CS Pin

If you use a different pin for Chip Select, update the following in `spi.c`:

```c
#define GPIOX     GPIOA   // GPIO port
#define CSS_PIN   4       // Pin number
```

---

## API Reference

### Management

| Function | Description |
|----------|-------------|
| `void w25q_write_enable()` | Enables write/erase operations. Must be called before any program or erase command. |
| `void w25q_power_down()` | Puts the device into low-power standby mode. |
| `uint8_t w25q_release_power_down()` | Wakes the device from power-down. Returns the device ID. |
| `void w25q_reset()` | Performs a software reset of the device. |

### Read

| Function | Description |
|----------|-------------|
| `uint16_t w25q_read_status_reg()` | Reads both status registers. SR1 in low byte, SR2 in high byte. |
| `void w25q_read(uint32_t addr, uint8_t *buffer, uint32_t length)` | Reads `length` bytes starting from `addr` into `buffer`. |
| `void w25q_read_security_reg(uint32_t addr, uint8_t *buffer, uint16_t length)` | Reads from one of the three 256-byte security registers. |
| `uint32_t w25q_read_jedec_id()` | Returns the 24-bit JEDEC ID: `[Manufacturer][MemType][Capacity]`. |

### Write & Erase

| Function | Description |
|----------|-------------|
| `uint8_t w25q_program_page(uint32_t addr, uint8_t *data, uint16_t length)` | Writes up to 256 bytes to a page. Returns `1` if data would overflow the page boundary. |
| `uint8_t w25q_program_sec_reg(uint32_t addr, uint8_t *data, uint16_t length)` | Programs a security register. Returns `1` on boundary overflow. |
| `void w25q_sector_erase(uint32_t addr)` | Erases 4KB sector containing `addr`. Blocks until complete. |
| `void w25q_block_erase_32k(uint32_t addr)` | Erases 32KB block containing `addr`. Blocks until complete. |
| `void w25q_block_erase_64k(uint32_t addr)` | Erases 64KB block containing `addr`. Blocks until complete. |
| `void w25q_erase_sec_reg(uint32_t addr)` | Erases the security register at the given address. Blocks until complete. |

### Security Register Addresses

```c
W25Q80BV_ADDR_SEC_REG1   // 0x001000 – 0x0010FF
W25Q80BV_ADDR_SEC_REG2   // 0x002000 – 0x0020FF
W25Q80BV_ADDR_SEC_REG3   // 0x003000 – 0x0030FF
```

---

## SPI Configuration

The driver initializes SPI1 as master. The configuration is passed via the `SPI_CONFIG` struct:

```c
SPI_CONFIG conf = {
    .baudrate = SPI_BRDIV2,     // PCLK/2
    .bitorder = SPI_MSB_FIRST,
    .mode     = SPI_MODE0       // CPOL=0, CPHA=0
};

spi1_init(&conf);
```

W25Q80BV supports SPI Mode 0 and Mode 3.

---

## File Structure

```
├── spi.h / spi.c               # SPI1 driver (register-level)
├── w25q80bv.h / w25q80bv.c     # W25Q80BV flash driver
├── w25q80bv_instructions.h     # Full command set definitions
├── main.h / main.c             # Example usage
```

---

## Device Info

| Property         | Value                    |
|------------------|--------------------------|
| Device           | Winbond W25Q80BV         |
| Capacity         | 8 Mbit (1 MB)            |
| Page Size        | 256 bytes                |
| Sector Size      | 4 KB                     |
| Block Size       | 32 KB / 64 KB            |
| Interface        | SPI (Mode 0 & 3)         |
| Max Clock        | 104 MHz                  |
| Supply Voltage   | 2.7V – 3.6V              |
