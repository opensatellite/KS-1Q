#ifndef IAP_H
#define IAP_H

#define IAP_Start_Sector_1
#define IAP_End_Sector_1
#define IAP_MAX_LEN_1				0xffff

#define IAP_Start_Sector_2
#define IAP_End_Sector_2
#define IAP_MAX_LEN_2				0xffff

typedef enum 
{
	IAP_NOERR		= 0,
	IAP_NOMEM		= 1,
	IAP_PROGERR		= 2,
	IAP_ERASEERR	= 3,
	IAP_TIMEOUT		= 4,
	IAP_TIMEOUT_RST	= 5,
	IAP_VERIFYERR	= 6
}IAP_STATUS;

IAP_STATUS iap_check_len(size_t len);
IAP_STATUS iap_add_lock();
IAP_STATUS iap_release_lock();
IAP_STATUS iap_erase_app();
IAP_STATUS iap_prog_app(uint32_t app_addr, uint32_t* data,size_t app_size);
IAP_STATUS iap_verify_app(uint32_t app_addr, uint32_t* data, size_t app_size);

#endif 