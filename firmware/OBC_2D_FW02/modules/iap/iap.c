#include <os.h>
#include "iap.h"

static uint8_t bin_running = 0;

IAP_STATUS iap_check_len(size_t len)
{
	if (bin_running)
	{
		if (len > IAP_MAX_LEN_1)
		{
			return IAP_NOMEM;
		}
		return IAP_NOERR;
	}
	if (len > IAP_MAX_LEN_2)
	{
		return IAP_NOMEM;
	}
	return IAP_NOERR;
}

IAP_STATUS iap_add_lock()
{
	FLASH_Lock();
	return IAP_NOERR;
}

IAP_STATUS iap_release_lock()
{
	FLASH_Unlock();
	return IAP_NOERR;
}

IAP_STATUS iap_erase_app()
{
	uint32_t erase_sector;
	uint8_t  erase_count;

	if (bin_running)
	{
		erase_sector = FLASH_Sector_2;
		erase_count = 7;
	}
	else
	{
		erase_sector = FLASH_Sector_9;
		erase_count = 3;
	}

	for (uint8_t n = 0; n < erase_count; n++)
	{
		if (FLASH_EraseSector(erase_sector,VoltageRange_3) == FLASH_COMPLETE)
		{
			erase_sector += 8;
		}
		else
		{
#ifdef IAP_DEBUG
			printf("FLASH ERASE ERR! Sector is %x\n\n", erase_sector);
#endif
			return IAP_ERASEERR;
		}
	}
	return IAP_NOERR;
}

IAP_STATUS iap_prog_app(uint32_t app_addr, uint32_t* data,size_t app_size)
{
	for (uint32_t n = 0; n < app_size; n++)
	{
		if (FLASH_ProgramWord(app_addr, *data) != FLASH_COMPLETE)
		{
#ifdef IAP_DEBUG
			printf("FLASH PROGRAM ERR! Addr is %x\n\n", app_addr);
#endif
			return IAP_PROGERR;
		}
		data++;
		app_addr += 4;
	}
	return IAP_NOERR;
}

IAP_STATUS iap_verify_app(uint32_t app_addr, uint32_t* data, size_t app_size)
{
	uint32_t app_data;
	for (uint32_t n = 0; n < app_size; n++)
	{
		app_data = *(uint32_t *)app_addr;
		if (app_data != *data)
		{
#ifdef IAP_DEBUG
			printf("FLASH Verify ERR! Addr is %x\n\n", app_addr);
#endif
			return IAP_VERIFYERR;
		}
		data++;
		app_addr++;
	}
	return IAP_NOERR;
}
