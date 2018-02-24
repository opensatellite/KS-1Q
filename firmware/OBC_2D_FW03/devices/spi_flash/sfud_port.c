/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2016-04-23
 */

#include <sfud.h>
#include <stdarg.h>

#include <os.h>
#include "main.h"

static char log_buf[512];

void sfud_log_debug(const char *file, const long line, const char *format, ...);

static void retry_delay_100us(void) {
	uint32_t delay = 120;
	while (delay--);
}


/**
 * SPI write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
        size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
    uint8_t send_data, read_data;

	if (write_size) {
		SFUD_ASSERT(write_buf);
	}
	if (read_size) {
		SFUD_ASSERT(read_buf);
	}
	
	osMutexWait(mutex_rom, osWaitForever);
	pinhigh((pin)PIN_SD_CS);
	
	spi_init(SPI_SD,0,8,256);
	pinlow((pin)PIN_FLASH_CS);
	for (size_t i = 0, retry_times; i < write_size + read_size; i++)
	{
		if (i < write_size)
			send_data = *write_buf++;
		else
			send_data = SFUD_DUMMY_DATA;
		retry_times = 1000;
		while (SPI_I2S_GetFlagStatus(SPI_SD, SPI_I2S_FLAG_TXE) == RESET)
		{
			SFUD_RETRY_PROCESS(NULL, retry_times, result);
		}
		if (result != SFUD_SUCCESS)
			goto exit;
		SPI_I2S_SendData(SPI_SD, send_data);

		retry_times = 1000;
		while (SPI_I2S_GetFlagStatus(SPI_SD, SPI_I2S_FLAG_RXNE) == RESET)
		{
			SFUD_RETRY_PROCESS(NULL, retry_times, result);
		}
		if (result != SFUD_SUCCESS)
			goto exit;
		read_data = SPI_I2S_ReceiveData(SPI_SD);
		if (i >= write_size) {
			*read_buf++ = read_data;
		}
	}

exit:
	pinhigh((pin)PIN_FLASH_CS);
	osMutexRelease(mutex_rom);
	
    return result;
}

//static spi_user_data spi3 = { .spix = SPI3,.cs_gpiox = GPIOA,.cs_gpio_pin = GPIO_Pin_15 };
sfud_err sfud_spi_port_init(sfud_flash *flash) 
{
    sfud_err result = SFUD_SUCCESS;
	switch (flash->index)
	{
	case SFUD_W25Q256_DEVICE_INDEX:
		//pinmode((pin)C_PA15, PUSHPULL);
		pinmode(PA15,PUSHPULL);

		flash->spi.wr = spi_write_read;
		flash->spi.lock = NULL;
		flash->spi.unlock = NULL;
		flash->spi.user_data = NULL;
		/* 100 microsecond delay */
		flash->retry.delay = retry_delay_100us;
		/* 60 seconds timeout */
		flash->retry.times = 60 * 10000;
		break;
	}

    return result;
}

/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void sfud_log_debug(const char *file, const long line, const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD](%s:%ld) ", file, line);
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\n", log_buf);
    va_end(args);
}

/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void sfud_log_info(const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD]");
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\n", log_buf);
    va_end(args);
}
