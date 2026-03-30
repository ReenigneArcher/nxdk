#ifndef __WINSOCK2_H__
#define __WINSOCK2_H__

#include <errno.h>
#include <minwinbase.h>
#include <windef.h>
#include <winerror.h>

#define mem_init nxdk_lwip_mem_init
#define mem_trim nxdk_lwip_mem_trim
#define mem_malloc nxdk_lwip_mem_malloc
#define mem_calloc nxdk_lwip_mem_calloc
#define mem_free nxdk_lwip_mem_free
#include "../../net/lwip/src/include/lwip/inet.h"
#include "../../net/lwip/src/include/lwip/netdb.h"
#include "../../net/lwip/src/include/lwip/sockets.h"
#undef mem_free
#undef mem_calloc
#undef mem_malloc
#undef mem_trim
#undef mem_init

#ifndef NXDK_LWIP_SOCKET_WRAPPERS_DEFINED
#define NXDK_LWIP_SOCKET_WRAPPERS_DEFINED 1

#ifdef accept
#undef accept
static inline int accept(int s, struct sockaddr *addr, socklen_t *addrlen) { return lwip_accept(s, addr, addrlen); }
#endif
#ifdef bind
#undef bind
static inline int bind(int s, const struct sockaddr *name, socklen_t namelen) { return lwip_bind(s, name, namelen); }
#endif
#ifdef shutdown
#undef shutdown
static inline int shutdown(int s, int how) { return lwip_shutdown(s, how); }
#endif
#ifdef getpeername
#undef getpeername
static inline int getpeername(int s, struct sockaddr *name, socklen_t *namelen) { return lwip_getpeername(s, name, namelen); }
#endif
#ifdef getsockname
#undef getsockname
static inline int getsockname(int s, struct sockaddr *name, socklen_t *namelen) { return lwip_getsockname(s, name, namelen); }
#endif
#ifdef setsockopt
#undef setsockopt
static inline int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen) { return lwip_setsockopt(s, level, optname, optval, optlen); }
#endif
#ifdef getsockopt
#undef getsockopt
static inline int getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen) { return lwip_getsockopt(s, level, optname, optval, optlen); }
#endif
#ifdef closesocket
#undef closesocket
static inline int closesocket(int s) { return lwip_close(s); }
#endif
#ifdef connect
#undef connect
static inline int connect(int s, const struct sockaddr *name, socklen_t namelen) { return lwip_connect(s, name, namelen); }
#endif
#ifdef listen
#undef listen
static inline int listen(int s, int backlog) { return lwip_listen(s, backlog); }
#endif
#ifdef recv
#undef recv
static inline ssize_t recv(int s, void *mem, size_t len, int flags) { return lwip_recv(s, mem, len, flags); }
#endif
#ifdef recvmsg
#undef recvmsg
static inline ssize_t recvmsg(int s, struct msghdr *message, int flags) { return lwip_recvmsg(s, message, flags); }
#endif
#ifdef recvfrom
#undef recvfrom
static inline ssize_t recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen) { return lwip_recvfrom(s, mem, len, flags, from, fromlen); }
#endif
#ifdef send
#undef send
static inline ssize_t send(int s, const void *dataptr, size_t size, int flags) { return lwip_send(s, dataptr, size, flags); }
#endif
#ifdef sendmsg
#undef sendmsg
static inline ssize_t sendmsg(int s, const struct msghdr *message, int flags) { return lwip_sendmsg(s, message, flags); }
#endif
#ifdef sendto
#undef sendto
static inline ssize_t sendto(int s, const void *dataptr, size_t size, int flags, const struct sockaddr *to, socklen_t tolen) { return lwip_sendto(s, dataptr, size, flags, to, tolen); }
#endif
#ifdef socket
#undef socket
static inline int socket(int domain, int type, int protocol) { return lwip_socket(domain, type, protocol); }
#endif
#ifdef select
#undef select
static inline int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout) { return lwip_select(maxfdp1, readset, writeset, exceptset, timeout); }
#endif
#ifdef ioctlsocket
#undef ioctlsocket
static inline int ioctlsocket(int s, long cmd, void *argp) { return lwip_ioctl(s, cmd, argp); }
#endif
#ifdef inet_ntop
#undef inet_ntop
static inline const char *inet_ntop(int af, const void *src, char *dst, socklen_t size) { return lwip_inet_ntop(af, src, dst, size); }
#endif
#ifdef inet_pton
#undef inet_pton
static inline int inet_pton(int af, const char *src, void *dst) { return lwip_inet_pton(af, src, dst); }
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WSAAPI
#define WSAAPI WINAPI
#endif

#ifndef SOMAXCONN
#define SOMAXCONN 0x7fffffff
#endif

#ifndef MSG_PARTIAL
#define MSG_PARTIAL 0x8000
#endif

#ifndef SD_RECEIVE
#define SD_RECEIVE SHUT_RD
#endif

#ifndef SD_SEND
#define SD_SEND SHUT_WR
#endif

#ifndef SD_BOTH
#define SD_BOTH SHUT_RDWR
#endif

#ifndef MAKEWORD
#define MAKEWORD(low, high) ((WORD)(((BYTE)((low) & 0xff)) | ((WORD)((BYTE)((high) & 0xff))) << 8))
#endif

#ifndef LOBYTE
#define LOBYTE(word) ((BYTE)((word) & 0xff))
#endif

#ifndef HIBYTE
#define HIBYTE(word) ((BYTE)(((word) >> 8) & 0xff))
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#ifndef IP_MTU_DISCOVER
#define IP_MTU_DISCOVER 71
#endif

#ifndef IP_PMTUDISC_DONT
#define IP_PMTUDISC_DONT 0
#endif

#ifndef IPV6_MTU_DISCOVER
#define IPV6_MTU_DISCOVER 71
#endif

#ifndef IPV6_PKTINFO
#define IPV6_PKTINFO 50
#endif

#ifndef IP_ECN
#define IP_ECN 50
#endif

#ifndef IPV6_ECN
#define IPV6_ECN 50
#endif

#ifndef WSA_IO_PENDING
#define WSA_IO_PENDING ERROR_IO_PENDING
#endif

#ifndef FIONREAD
#define FIONREAD _IOR('f', 127, unsigned long)
#endif

#ifndef FIONBIO
#define FIONBIO _IOW('f', 126, unsigned long)
#endif

#ifndef IOC_WS2
#define IOC_WS2 0x08000000UL
#endif

#ifndef IOC_VENDOR
#define IOC_VENDOR 0x18000000UL
#endif

#ifndef _WSAIO
#define _WSAIO(x, y) (IOC_VOID | ((x) << 8) | (y))
#endif

#ifndef _WSAIOR
#define _WSAIOR(x, y) (IOC_OUT | ((x) << 8) | (y))
#endif

#ifndef _WSAIOW
#define _WSAIOW(x, y) (IOC_IN | ((x) << 8) | (y))
#endif

#ifndef _WSAIORW
#define _WSAIORW(x, y) (IOC_INOUT | ((x) << 8) | (y))
#endif

#ifndef SIO_UDP_CONNRESET
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
#endif

typedef int SOCKET;
typedef fd_set FD_SET;
typedef struct timeval TIMEVAL;
typedef struct hostent HOSTENT;
typedef struct linger LINGER;
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr *PSOCKADDR;
typedef struct sockaddr *LPSOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr_in *PSOCKADDR_IN;
typedef struct sockaddr_in6 SOCKADDR_IN6;
typedef struct sockaddr_in6 *PSOCKADDR_IN6;
typedef struct sockaddr_storage SOCKADDR_STORAGE;
typedef struct sockaddr_storage *PSOCKADDR_STORAGE;
typedef struct in_addr IN_ADDR;
typedef struct in6_addr IN6_ADDR;
typedef struct addrinfo ADDRINFOA;
typedef struct addrinfo *PADDRINFOA;
typedef struct addrinfo *LPADDRINFOA;
typedef unsigned int GROUP;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short u_short;

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

#ifndef NXDK_SERVENT_DEFINED
#define NXDK_SERVENT_DEFINED 1
struct servent {
    char *s_name;
    char **s_aliases;
    int s_port;
    char *s_proto;
};
#endif

typedef struct WSAData {
    WORD wVersion;
    WORD wHighVersion;
    char szDescription[257];
    char szSystemStatus[129];
    unsigned short iMaxSockets;
    unsigned short iMaxUdpDg;
    char *lpVendorInfo;
} WSADATA, *LPWSADATA;

typedef struct _WSAPROTOCOL_INFOA {
    DWORD dwServiceFlags1;
} WSAPROTOCOL_INFOA, *LPWSAPROTOCOL_INFOA;

typedef struct _WSABUF {
    ULONG len;
    CHAR *buf;
} WSABUF, *LPWSABUF;

typedef struct _WSAMSG {
    LPSOCKADDR name;
    INT namelen;
    LPWSABUF lpBuffers;
    DWORD dwBufferCount;
    WSABUF Control;
    DWORD dwFlags;
} WSAMSG, *LPWSAMSG;

typedef struct cmsghdr WSACMSGHDR;
typedef WSACMSGHDR *PWSACMSGHDR;

typedef INT (WSAAPI *LPWSAOVERLAPPED_COMPLETION_ROUTINE)(DWORD, DWORD, LPOVERLAPPED, DWORD);

typedef INT (WSAAPI *LPFN_WSARECVMSG)(SOCKET, LPWSAMSG, LPDWORD, LPOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef INT (WSAAPI *LPFN_WSASENDMSG)(SOCKET, LPWSAMSG, DWORD, LPDWORD, LPOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);

#define WSA_CMSG_FIRSTHDR(mhdr) ((PWSACMSGHDR)CMSG_FIRSTHDR((struct msghdr *)(mhdr)))
#define WSA_CMSG_NXTHDR(mhdr, cmsg) ((PWSACMSGHDR)CMSG_NXTHDR((struct msghdr *)(mhdr), (struct cmsghdr *)(cmsg)))
#define WSA_CMSG_DATA(cmsg) CMSG_DATA((struct cmsghdr *)(cmsg))
#define WSA_CMSG_SPACE(length) CMSG_SPACE(length)
#define WSA_CMSG_LEN(length) CMSG_LEN(length)

int WSAAPI WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
int WSAAPI WSACleanup(void);
int WSAAPI WSAGetLastError(void);
void WSAAPI WSASetLastError(int iError);
SOCKET WSAAPI WSASocketA(int af, int type, int protocol, LPWSAPROTOCOL_INFOA lpProtocolInfo, GROUP g, DWORD dwFlags);
int WSAAPI WSAIoctl(SOCKET s, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer,
                    LPVOID lpvOutBuffer, DWORD cbOutBuffer, LPDWORD lpcbBytesReturned,
                    LPOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
INT WSAAPI WSARecvMsg(SOCKET s, LPWSAMSG lpMsg, LPDWORD lpdwNumberOfBytesRecvd,
                      LPOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
INT WSAAPI WSASendMsg(SOCKET s, LPWSAMSG lpMsg, DWORD dwFlags, LPDWORD lpNumberOfBytesSent,
                      LPOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
int getnameinfo(const struct sockaddr *addr, socklen_t addrlen, char *host, socklen_t hostlen,
                char *serv, socklen_t servlen, int flags);
const char *gai_strerror(int errcode);
struct servent *getservbyname(const char *name, const char *proto);

#ifdef __cplusplus
}
#endif

#endif
