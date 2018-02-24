#ifndef SPI_FLASH_H
#define SPI_FLASH_H

#include <os.h>

#include <sfud.h>

#include "spiffs.h"

extern spiffs spi_flash_fs;

int spi_flash_init(void);

#endif