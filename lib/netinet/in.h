#ifndef NXDK_NETINET_IN_H
#define NXDK_NETINET_IN_H

#define mem_init nxdk_lwip_mem_init
#define mem_trim nxdk_lwip_mem_trim
#define mem_malloc nxdk_lwip_mem_malloc
#define mem_calloc nxdk_lwip_mem_calloc
#define mem_free nxdk_lwip_mem_free
#include "../net/lwip/src/include/lwip/inet.h"
#include "../net/lwip/src/include/lwip/sockets.h"
#undef mem_free
#undef mem_calloc
#undef mem_malloc
#undef mem_trim
#undef mem_init
#ifdef accept
#undef accept
#endif
#ifdef bind
#undef bind
#endif
#ifdef shutdown
#undef shutdown
#endif
#ifdef getpeername
#undef getpeername
#endif
#ifdef getsockname
#undef getsockname
#endif
#ifdef setsockopt
#undef setsockopt
#endif
#ifdef getsockopt
#undef getsockopt
#endif
#ifdef closesocket
#undef closesocket
#endif
#ifdef connect
#undef connect
#endif
#ifdef listen
#undef listen
#endif
#ifdef recv
#undef recv
#endif
#ifdef recvmsg
#undef recvmsg
#endif
#ifdef recvfrom
#undef recvfrom
#endif
#ifdef send
#undef send
#endif
#ifdef sendmsg
#undef sendmsg
#endif
#ifdef sendto
#undef sendto
#endif
#ifdef socket
#undef socket
#endif
#ifdef select
#undef select
#endif
#ifdef poll
#undef poll
#endif
#ifdef ioctl
#undef ioctl
#endif
#ifdef ioctlsocket
#undef ioctlsocket
#endif

#ifndef IPV6_PKTINFO
#define IPV6_PKTINFO 50
#endif

#ifndef IPV6_RECVPKTINFO
#define IPV6_RECVPKTINFO IPV6_PKTINFO
#endif

#ifndef NXDK_IN6_PKTINFO_DEFINED
#define NXDK_IN6_PKTINFO_DEFINED 1
struct in6_pktinfo {
    struct in6_addr ipi6_addr;
    unsigned int ipi6_ifindex;
};
#endif

#ifndef ipi_spec_dst
#define ipi_spec_dst ipi_addr
#endif

#endif
