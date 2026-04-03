#ifndef NXDK_NETDB_H
#define NXDK_NETDB_H

#include "net/lwip/src/include/compat/posix/netdb.h"

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

#ifndef NXDK_SERVENT_DEFINED
#define NXDK_SERVENT_DEFINED 1
struct servent {
	char *s_name;
	char **s_aliases;
	int s_port;
	char *s_proto;
};
#endif

int getnameinfo(const struct sockaddr *addr, socklen_t addrlen, char *host, socklen_t hostlen,
				char *serv, socklen_t servlen, int flags);
const char *gai_strerror(int errcode);
struct servent *getservbyname(const char *name, const char *proto);

#ifdef __cplusplus
}
#endif

#endif
