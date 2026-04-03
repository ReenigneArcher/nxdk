#ifndef __WS2TCPIP_H__
#define __WS2TCPIP_H__

#include <netdb.h>
#include <winsock2.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

#ifndef NI_MAXSERV
#define NI_MAXSERV 32
#endif

#ifndef NI_NUMERICHOST
#define NI_NUMERICHOST 0x01
#endif

#ifndef NI_NUMERICSERV
#define NI_NUMERICSERV 0x02
#endif

typedef struct sockaddr_storage SOCKADDR_STORAGE_LH;
typedef struct addrinfo ADDRINFO;
typedef struct addrinfo *PADDRINFO;
typedef const struct addrinfo *PCADDRINFO;

typedef struct in_pktinfo IN_PKTINFO;

#ifndef NXDK_SERVENT_DEFINED
#define NXDK_SERVENT_DEFINED 1
struct servent {
    char *s_name;
    char **s_aliases;
    int s_port;
    char *s_proto;
};
#endif

#ifndef NXDK_IN6_PKTINFO_DEFINED
#define NXDK_IN6_PKTINFO_DEFINED 1
typedef struct in6_pktinfo {
    struct in6_addr ipi6_addr;
    unsigned int ipi6_ifindex;
} IN6_PKTINFO, *PIN6_PKTINFO;
#else
typedef struct in6_pktinfo IN6_PKTINFO;
typedef IN6_PKTINFO *PIN6_PKTINFO;
#endif

static inline const char *InetNtopA(int family, const void *address, char *stringBuffer, size_t stringBufferSize)
{
    return inet_ntop(family, address, stringBuffer, (socklen_t)stringBufferSize);
}

static inline int InetPtonA(int family, const char *pszAddrString, void *pAddrBuf)
{
    return inet_pton(family, pszAddrString, pAddrBuf);
}

int getnameinfo(const struct sockaddr *addr, socklen_t addrlen, char *host, socklen_t hostlen,
                char *serv, socklen_t servlen, int flags);
const char *gai_strerror(int errcode);
struct servent *getservbyname(const char *name, const char *proto);

#ifndef InetNtop
#define InetNtop InetNtopA
#endif

#ifndef InetPton
#define InetPton InetPtonA
#endif

#ifdef __cplusplus
}
#endif

#endif
