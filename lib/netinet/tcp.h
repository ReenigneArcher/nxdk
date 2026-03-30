#ifndef NXDK_NETINET_TCP_H
#define NXDK_NETINET_TCP_H

#define mem_init nxdk_lwip_mem_init
#define mem_trim nxdk_lwip_mem_trim
#define mem_malloc nxdk_lwip_mem_malloc
#define mem_calloc nxdk_lwip_mem_calloc
#define mem_free nxdk_lwip_mem_free
#include "../net/lwip/src/include/lwip/sockets.h"
#undef mem_free
#undef mem_calloc
#undef mem_malloc
#undef mem_trim
#undef mem_init

#endif
