/* WARNING! All changes made to this file will be lost! */

#ifdef __GNUC__

#define PACKED 	__attribute__((__packed__))
#define WEAK	__attribute__((weak))
#define CONST   CONST

#elif __ICCARM__

#define PACKED	__packed
#define WEAK	__weak
#define CONST

//#pragma pack(8)

#endif


#ifndef _INCLUDE_CSP_CSP_AUTOCONFIG_H_WAF
#define _INCLUDE_CSP_CSP_AUTOCONFIG_H_WAF

#define GIT_REV "unknown"
#define CSP_FREERTOS
/* #define CSP_POSIX 1 */
/* #undef CSP_WINDOWS */
/* #undef CSP_MACOSX */
#define CSP_DEBUG 1
/* #undef CSP_VERBOSE */
//#define CSP_USE_RDP
#define CSP_USE_CRC32
//#define CSP_USE_HMAC
//#define CSP_USE_XTEA
/* #undef CSP_USE_PROMISC */
/* #undef CSP_USE_QOS */
/* #undef CSP_USE_DEDUP */
#define CSP_CONN_MAX 10
#define CSP_CONN_QUEUE_LENGTH 100
#define CSP_FIFO_INPUT 10
#define CSP_MAX_BIND_PORT 31
#define CSP_RDP_MAX_WINDOW 20
#define CSP_PADDING_BYTES 8
#define CSP_TRANSACTION_SO 0
#define CSP_CONNECTION_SO	0
#define CSP_LOG_LEVEL_DEBUG 1
#define CSP_LOG_LEVEL_INFO 1
#define CSP_LOG_LEVEL_WARN 1
#define CSP_LOG_LEVEL_ERROR 1
#define CSP_LITTLE_ENDIAN 1
/* #undef CSP_BIG_ENDIAN */
#define CSP_HAVE_STDBOOL_H 1

#endif /* _INCLUDE_CSP_CSP_AUTOCONFIG_H_WAF */
