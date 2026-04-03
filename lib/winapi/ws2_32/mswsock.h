#ifndef __MSWSOCK_H__
#define __MSWSOCK_H__

#include <guiddef.h>
#include <winsock2.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IOC_WS2
#define IOC_WS2 0x08000000UL
#endif

#ifndef IOC_VENDOR
#define IOC_VENDOR 0x18000000UL
#endif

#ifndef SIO_GET_EXTENSION_FUNCTION_POINTER
#define SIO_GET_EXTENSION_FUNCTION_POINTER (IOC_INOUT | IOC_WS2 | 6)
#endif

extern const GUID WSAID_WSARECVMSG;
extern const GUID WSAID_WSASENDMSG;

#ifdef __cplusplus
}
#endif

#endif
