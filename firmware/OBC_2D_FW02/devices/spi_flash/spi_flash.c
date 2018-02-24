#include <os.h>
#include "spi_flash.h"

#include <sfud.h>

#include "spiffs.h"

spiffs spi_flash_fs;

static uint8_t FS_Work_Buf[SPIFLASH_CFG_LOG_PAGE_SZ * 2];
static uint8_t FS_FDS[32 * 4];
static uint8_t FS_Cache_Buf[(SPIFLASH_CFG_LOG_PAGE_SZ + 32) * 4];

static int32_t spi_flash_read(uint32_t addr, uint32_t size, uint8_t *dst)
{
	const sfud_flash *flash = sfud_get_device_table() + 0;

	sfud_read(flash, addr, size, dst);
	return 0;
}

static int32_t spi_flash_write(uint32_t addr, uint32_t size, uint8_t *src)
{
	const sfud_flash *flash = sfud_get_device_table() + 0;

	sfud_write(flash, addr, size, src);
	return 0;
}

static int32_t spi_flash_earse(uint32_t addr, uint32_t size)
{
	const sfud_flash *flash = sfud_get_device_table() + 0;

	sfud_erase(flash, addr, size);
	return 0;
}

int spi_flash_init()
{
	int ret;
	
	spiffs_config SPIFlashCfg;
	SPIFlashCfg.phys_size = SPIFLASH_CFG_PHYS_SZ;
	SPIFlashCfg.phys_addr = SPIFLASH_CFG_PHYS_ADDR;
	SPIFlashCfg.phys_erase_block = SPIFLASH_CFG_PHYS_ERASE_SZ;
	SPIFlashCfg.log_block_size = SPIFLASH_CFG_LOG_BLOCK_SZ;
	SPIFlashCfg.log_page_size = SPIFLASH_CFG_LOG_PAGE_SZ;

	SPIFlashCfg.hal_read_f = spi_flash_read;
	SPIFlashCfg.hal_write_f = spi_flash_write;
	SPIFlashCfg.hal_erase_f = spi_flash_earse;

	ret = sfud_init();
	if(ret != 0)
	{
		return 0;
	}
	
	printf("Try to mount SPI Flash ...... ");
	ret = SPIFFS_mount(&spi_flash_fs,
			&SPIFlashCfg,
			FS_Work_Buf,
			FS_FDS,
			sizeof(FS_FDS),
			FS_Cache_Buf,
			sizeof(FS_Cache_Buf),
			0);
	
	if(ret != 0)
	{
		return 0;
	}
	printf("Success\n");
	return 1;
}